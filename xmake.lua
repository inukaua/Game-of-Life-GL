add_requires("glfw3")

target("GOL")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glfw3")
    add_includedirs("include")

