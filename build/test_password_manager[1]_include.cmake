if(EXISTS "/Users/mainlarbibenamour/desktop/password_manager-2/build/test_password_manager[1]_tests.cmake")
  include("/Users/mainlarbibenamour/desktop/password_manager-2/build/test_password_manager[1]_tests.cmake")
else()
  add_test(test_password_manager_NOT_BUILT test_password_manager_NOT_BUILT)
endif()