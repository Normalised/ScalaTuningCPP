@REM Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
@REM or copy at http://opensource.org/licenses/MIT)
mkdir build
cd build

@REM Generate a Visual Studio solution for latest version found
cmake -DSCALATUNINGCPP_BUILD_TESTS=1 -G "Visual Studio 15 2017 Win64" ..

@REM Build default configuration (ie 'Debug')
cmake --build .

ctest --output-on-failure

cd ..