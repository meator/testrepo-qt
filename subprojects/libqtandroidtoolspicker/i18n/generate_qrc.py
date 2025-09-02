#!/usr/bin/env python3

import xml.etree.ElementTree as ET
import argparse
import sys

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("filenames", nargs='+')
    args = parser.parse_args()

    root = ET.Element("RCC")
    qresource = ET.SubElement(root, "qresource")
    qresource.set("prefix", "/i18n")

    for filename in args.filenames:
        assert filename.endswith(".ts")
        file = ET.SubElement(qresource, "file")
        file.text = filename.removesuffix(".ts") + ".qm"

    tree = ET.ElementTree(root)
    tree.write(sys.stdout, encoding="unicode")
