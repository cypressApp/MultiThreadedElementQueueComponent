#!/usr/bin/env python3
from kconfiglib import Kconfig
import os

kconf = Kconfig("Kconfig")

if not os.path.exists(".config"):
    kconf.write_config(".config")

kconf.load_config(".config")
kconf.write_autoconf("autoconf.h")