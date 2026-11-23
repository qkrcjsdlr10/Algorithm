"""Entry point that works without installing the package: python tools/algo_cli.py ..."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from algo.cli import main  # noqa: E402

raise SystemExit(main())
