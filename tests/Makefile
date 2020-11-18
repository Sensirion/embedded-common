include ./default_config.inc

embedded_common_test_binaries := embedded-common-test

.PHONY: all clean test

all: clean test

embedded-common-test: embedded-common-test.cpp ${sensirion_i2c_sources} ${sensirion_shdlc_sources} ${sensirion_common_sources} ${sensirion_test_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ${embedded_common_test_binaries}

test: ${embedded_common_test_binaries}
	set -ex; for test in ${embedded_common_test_binaries}; do echo $${test}; ./$${test}; echo; done;
