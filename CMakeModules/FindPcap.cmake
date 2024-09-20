find_path(PCAP_INCLUDE_DIR pcap.h)
find_library(PCAP_LIBRARY NAMES pcap)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pcap DEFAULT_MSG PCAP_LIBRARY PCAP_INCLUDE_DIR)

if(PCAP_LIBRARY AND PCAP_INCLUDE_DIR)
  set(PCAP_FOUND TRUE)
else()
  set(PCAP_FOUND FALSE)
endif()

if(PCAP_FOUND)
  set(PCAP_LIBRARIES ${PCAP_LIBRARY})
  set(PCAP_INCLUDE_DIRS ${PCAP_INCLUDE_DIR})
endif()

mark_as_advanced(PCAP_INCLUDE_DIR PCAP_LIBRARY)

