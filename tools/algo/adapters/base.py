"""Site adapter interface and support levels.

Support levels (per capability):
  SUPPORTED    officially documented and confirmed
  UNOFFICIAL   works through the signed-in site's own requests; terms need review
  UNSUPPORTED  not possible right now
  UNKNOWN      not verified
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass, field
from enum import Enum
from typing import Any, Iterator

from algo.ingest import FetchedSubmission


class Support(str, Enum):
    SUPPORTED = "SUPPORTED"
    UNOFFICIAL = "UNOFFICIAL"
    UNSUPPORTED = "UNSUPPORTED"
    UNKNOWN = "UNKNOWN"


CAPABILITIES = ("official_api", "submission_list", "verdict", "source_code", "auth_required")


class AdapterError(RuntimeError):
    pass


class LoginRequired(AdapterError):
    """The dedicated browser profile (or cookie) is not signed in. Never bypassed."""


class AccessLimited(AdapterError):
    """403/429/CAPTCHA or similar. The adapter stops and backs off; never bypassed."""


class Unavailable(AdapterError):
    """Prerequisite missing (browser not running, package not installed, ...)."""


@dataclass
class FetchContext:
    known_submission_keys: set[str]
    known_problem_ids: set[str]
    pending_problem_ids: set[str]
    latest_known_at: dict[str, str] = field(default_factory=dict)
    max_problems: int = 10
    delay_seconds: float = 1.0


class SiteAdapter:
    site: str = ""
    config_name: str = ""
    capabilities: dict[str, Support] = {}
    notes: str = ""

    def fetch(self, context: FetchContext) -> Iterator[FetchedSubmission]:
        raise NotImplementedError

    def close(self) -> None:
        pass


def runner_args(cdp_url: str, delay: float, output: Any) -> argparse.Namespace:
    """Arguments the existing backup scripts' BackupRunner classes expect."""
    return argparse.Namespace(cdp_url=cdp_url, delay=max(0.5, delay), output=output,
                              inspect=False, test=False, all=False, limit=None)


def translate_errors(module: Any, error: BaseException) -> AdapterError:
    """Map a backup script's exceptions onto adapter errors."""
    if isinstance(error, getattr(module, "AuthenticationError", ())):
        return LoginRequired(str(error))
    if isinstance(error, getattr(module, "AccessLimitedError", ())):
        return AccessLimited(str(error))
    return AdapterError(f"{type(error).__name__}: {error}")
