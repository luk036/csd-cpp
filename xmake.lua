add_rules("mode.debug", "mode.release", "mode.coverage")
add_requires("doctest", "fmt", "spdlog", "nanobench", "cxxopts")

set_languages("c++17")
add_includedirs("include")

if is_mode("release") then
    set_optimize("fastest")
end

if is_mode("coverage") then
    add_cxflags("-ftest-coverage", "-fprofile-arcs", {force = true})
end

if is_plat("linux") then
    set_warnings("all", "error")
    -- Termux/Android ships its libc++ headers under $PREFIX
    local prefix = os.getenv("PREFIX")
    if prefix then
        add_cxflags("-Wno-unused-command-line-argument", {force = true})
        add_sysincludedirs(prefix .. "/include/c++/v1", prefix .. "/include", {public = true})
    end
elseif is_plat("windows") then
    add_cxflags("/EHsc /W4 /WX /wd4702", {force = true})
end

target("Csd")
    set_kind("static")
    add_files("source/csd.cpp", "source/lcsre.cpp", "source/csd_multiplier.cpp")

target("check_verilog")
    add_deps("Csd")
    add_files("bench/check_verilog.cpp")
    add_packages("fmt")

target("bench_csd")
    add_deps("Csd")
    add_files("bench/bench_csd.cpp")
    add_packages("nanobench")

target("CsdLogger")
    set_kind("static")
    add_deps("Csd")
    add_files("source/logger.cpp")
    add_packages("spdlog")

target("test_csd")
    add_deps("Csd")
    add_files("test/source/*.cpp")
    add_packages("doctest")
    add_tests("default")

    -- rapidcheck is optionally provided by a CMake build (build_test/, build/).
    -- Its compiled configuration must match the xmake mode, otherwise the MSVC
    -- runtime (_ITERATOR_DEBUG_LEVEL, MD vs MDd) mismatches at link time.
    local rapidcheck_cfg = is_mode("debug") and "Debug" or "Release"
    local rapidcheck_dir = path.join(os.projectdir(), "build_test", "_deps", "rapidcheck-src")
    local rapidcheck_lib_dir = path.join(os.projectdir(), "build_test", "_deps", "rapidcheck-build")
    if is_plat("windows") then
        rapidcheck_lib_dir = path.join(rapidcheck_lib_dir, rapidcheck_cfg)
    end

    -- Fallback to build directory if build_test doesn't exist
    if not os.isdir(rapidcheck_dir) then
        rapidcheck_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-src")
        rapidcheck_lib_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-build")
        if is_plat("windows") then
            rapidcheck_lib_dir = path.join(rapidcheck_lib_dir, rapidcheck_cfg)
        end
    end

    if os.isdir(rapidcheck_dir) and os.isdir(rapidcheck_lib_dir) then
        add_includedirs(path.join(rapidcheck_dir, "include"))
        add_linkdirs(rapidcheck_lib_dir)
        add_links("rapidcheck")
        add_defines("RAPIDCHECK_H")
    end

target("test_switch")
    add_deps("Csd")
    add_files("bench/BM_switch.cpp")
    add_packages("nanobench")

target("csd_cli")
    add_deps("Csd")
    add_files("standalone/source/main.cpp")
    add_packages("cxxopts")

target("csd_cli_spdlog")
    add_deps("CsdLogger")
    add_files("standalone/source/main_with_spdlog.cpp")
    add_packages("cxxopts", "spdlog")
