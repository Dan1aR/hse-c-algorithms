.PHONY: \
	lint

BUILD_PATHS = src/*.cpp include/*.hpp

lint:
	clang-format -i --Werror --style=file ${BUILD_PATHS}
