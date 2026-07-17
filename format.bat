@echo off
REM Format the whole project with clang-format (in place).
REM Uses Git Bash's find/xargs (ships with Git for Windows) so the same
REM command works on Windows, macOS, and Linux.
REM Requires clang-format on PATH (install via: winget install LLVM.LLVM).

where clang-format >nul 2>&1
if errorlevel 1 (
    echo [ERROR] clang-format not found on PATH. Install it with: winget install LLVM.LLVM
    pause
    exit /b 1
)

bash -c "find . -type f \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.cxx' -o -name '*.cc' \) -not -path './build/*' -not -path './.cache/*' -not -path './.git/*' | xargs clang-format -i"

echo [OK] Formatting complete.
pause
