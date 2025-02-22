mkdir ../build
cd ../build
cmake .. -DUSE_CUDA=True -DCMAKE_BUILD_TYPE=RELEASE
cmake --build . --parallel 16
cmake --install .
cd ../deployment
dpkg-deb --build --root-owner-group deb_packages/JARVIS-AcquisitionTool_1.1-1_amd64_2004
mv deb_packages/JARVIS-AcquisitionTool_1.1-1_amd64_2004.deb .
