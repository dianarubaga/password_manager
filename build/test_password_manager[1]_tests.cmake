add_test([=[PasswordManagerTestSuite.AddNewPassword]=]  /Users/talineshawwa/Desktop/programming/password_manager/build/test_password_manager [==[--gtest_filter=PasswordManagerTestSuite.AddNewPassword]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[PasswordManagerTestSuite.AddNewPassword]=]  PROPERTIES WORKING_DIRECTORY /Users/talineshawwa/Desktop/programming/password_manager/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[PasswordManagerTestSuite.DeletePassword]=]  /Users/talineshawwa/Desktop/programming/password_manager/build/test_password_manager [==[--gtest_filter=PasswordManagerTestSuite.DeletePassword]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[PasswordManagerTestSuite.DeletePassword]=]  PROPERTIES WORKING_DIRECTORY /Users/talineshawwa/Desktop/programming/password_manager/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[PasswordManagerTestSuite.AddAfterDelete]=]  /Users/talineshawwa/Desktop/programming/password_manager/build/test_password_manager [==[--gtest_filter=PasswordManagerTestSuite.AddAfterDelete]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[PasswordManagerTestSuite.AddAfterDelete]=]  PROPERTIES WORKING_DIRECTORY /Users/talineshawwa/Desktop/programming/password_manager/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  test_password_manager_TESTS PasswordManagerTestSuite.AddNewPassword PasswordManagerTestSuite.DeletePassword PasswordManagerTestSuite.AddAfterDelete)
