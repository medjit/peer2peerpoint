# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jonny/esp/esp-idf/components/bootloader/subproject"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/tmp"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/src/bootloader-stamp"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/src"
  "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jonny/esp/esp_projects/peer2peerpoint/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
