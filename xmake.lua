set_project("GeoPixel")

set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
is_windows = is_plat("windows") or is_plat("mingw")

-- the debug mode
if is_mode("debug") then
    -- enable the debug symbols
    set_symbols("debug")
    -- disable optimization
    set_optimize("none")
end
-- the release mode
if is_mode("release") then
    -- set the symbols visibility: hidden
    set_symbols("hidden")
    -- enable fastest optimization
    set_optimize("fastest")
    -- strip all symbols
    set_strip("all")
end

add_repositories("local-repo /home/brickmaster/projects/ViXeL/xmake-transition/ViXeL/build")
add_requires("vixel", {alias = "ViXeL"})

target("GeoPixel")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    add_packages(
        "ViXeL"
    )
    local output_dir = path.join("$(buildir)", "$(os)", "$(arch)", "$(mode)")
    after_build(function (target)
        os.cp("resources", path.join(target:targetdir(), "resources"))
    end)
target_end()