# coding=utf-8

import os
import re

ROOT = r"."

HEAD = """cmake_minimum_required(VERSION 3.10)
project(Cpp_Concurrency_in_Action)
"""
TEMPL = """add_executable({TARGET} {SOURCE})
"""

PATTERN = "(.+)_(\d+|[ac])\.(\d+)"

def NormalizeTarget(name):
    print(name)
    m = re.search(PATTERN, name)
    assert m
    print(m.group())
    chap = m.group(2)
    if chap not in ['a', 'c']:
        chap = ("0" + chap)[-2:]
    sec = ("0" + m.group(3))[-2:]
    return m.group(1) + "_" + chap + "." + sec

def GenerateCMakeConfigFile():
    with open("CMakeLists.txt", "w") as f:
        f.write(HEAD)
        for file in os.listdir(ROOT):
            if file.endswith(".cpp"):
                target = file[:-4]
                target = NormalizeTarget(target)
                f.write(TEMPL.format(TARGET=target, SOURCE=file))

if __name__ == "__main__":
    GenerateCMakeConfigFile()
