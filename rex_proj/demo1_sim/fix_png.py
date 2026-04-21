#!/usr/bin/env python3
import sys
from PIL import Image

if len(sys.argv) != 2:
    print("用法: python fix_png.py <file.png>")
    sys.exit(1)

img = Image.open(sys.argv[1])
img.save(sys.argv[1], icc_profile=None)
print(f"已修复: {sys.argv[1]}")
