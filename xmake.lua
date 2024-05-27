add_requires("glfw3")

-- target("shader")
--     set_kind("object")
--     add_headerfiles("src/shader.h")
--     add_files("src/shader.cpp")
--     add_includedirs("include")


target("GOL")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glfw3")
    add_includedirs("include")

