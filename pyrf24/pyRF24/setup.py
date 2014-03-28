# Copyright 2013 Jonathon Grigg <jonathongrigg@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# setup.py, file based on Golgauth's answer
# http://stackoverflow.com/questions/16993927/ \
#    using-cython-to-link-python-to-a-shared-library
import os
import shutil
import sys

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

# clean previous build
for root, dirs, files in os.walk(".", topdown=False):
    for name in files:
        if (name.startswith("pyRF24") and not(name.endswith(".pyx") or
                name.endswith(".pxd"))):
            os.remove(os.path.join(root, name))
    for name in dirs:
        if (name == "build"):
            shutil.rmtree(name)

# build pyRF24
setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [
    Extension("pyRF24",
                sources=["pyRF24.pyx"],
                libraries=["rf24"],          # refers to installed "librf24.so"
                language="c++",
                extra_compile_args=["-I../librf24", "-fopenmp", "-O3"],
                extra_link_args=["-L./usr/local/lib"]
            )
    ]
)
# add it to '/usr/lib/pythonx.x/site-packages' for easy importing
# also rename it from the crazy pyRF24.cpython-32mu.so
# NOTE: requires sudo when running!
for files in os.listdir("."):
    if files.startswith("pyRF24") and files.endswith(".so"):
        shutil.copy(files, '/usr/lib/python{}.{}/site-packages/pyRF24.so'\
            .format(sys.version_info[0], sys.version_info[1]))
