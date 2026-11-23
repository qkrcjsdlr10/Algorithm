"""Source hashes used to decide whether a local file is the accepted submission.

Two kinds of hash are kept apart on purpose:

* ``raw``: SHA-256 of the exact bytes.
* ``normalized``: SHA-256 after a *conservative* normalization that only touches
  differences which never change Java/C/C++/SQL semantics:
    - a leading UTF-8 BOM is removed,
    - CRLF / CR line endings become LF,
    - trailing newlines at end of file collapse to exactly one.

Spaces, tabs and trailing spaces inside lines are never touched, so string
literals, text blocks and raw strings keep their meaning. Anything that only
matches after a more aggressive normalization is *not* a match.
"""

from __future__ import annotations

import hashlib

BOM = b"\xef\xbb\xbf"


def raw_hash(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def normalize(data: bytes) -> bytes:
    if data.startswith(BOM):
        data = data[len(BOM):]
    data = data.replace(b"\r\n", b"\n").replace(b"\r", b"\n")
    stripped = data.rstrip(b"\n")
    return stripped + b"\n" if stripped else b""


def normalized_hash(data: bytes) -> str:
    return hashlib.sha256(normalize(data)).hexdigest()


def line_ending_variants(data: bytes) -> set[str]:
    """Raw hashes of every byte form that differs from ``data`` only by the
    allowed normalization. Used when the site side is known only by its raw
    hash (for example ``sha256`` fields in existing metadata.json files)."""
    core = normalize(data)
    body = core.rstrip(b"\n")
    lf_forms = {body, body + b"\n"} if body else {b""}
    forms: set[bytes] = set()
    for form in lf_forms:
        forms.add(form)
        forms.add(form.replace(b"\n", b"\r\n"))
    forms |= {BOM + form for form in list(forms)}
    forms.add(data)
    return {raw_hash(form) for form in forms}


def match_kind(
    local: bytes, site_raw_hash: str | None, site_normalized_hash: str | None = None
) -> str | None:
    """Return ``"raw"`` or ``"normalized"`` when the local bytes are the site code,
    otherwise ``None``."""
    if site_raw_hash and raw_hash(local) == site_raw_hash:
        return "raw"
    if site_normalized_hash and normalized_hash(local) == site_normalized_hash:
        return "normalized"
    if site_raw_hash and site_raw_hash in line_ending_variants(local):
        return "normalized"
    return None
