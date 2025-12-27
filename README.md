# MuseumAfterDark - Unreal Engine Project

## Prerequisites

- **Unreal Engine 5.6** (or compatible version)
- **Visual Studio 2022** (or Visual Studio 2019/2022 with C++ development tools)
- **Windows 10/11** (64-bit)

## Initial Setup (After Downloading from GitHub)

This project uses both **C++ and Blueprints**. When you download the project from GitHub, you need to regenerate the project files and compile the C++ code before opening it in Unreal Engine.

### Option 1: Automatic Setup (Recommended - Builds Everything)

1. Navigate to the project folder: `MuseumAfterDark/`
2. Double-click `BuildProject.bat` (or right-click and select "Run as administrator")
3. Wait for the script to:
   - Generate Visual Studio project files
   - Build the C++ modules (this takes 5-15 minutes)
4. Once complete, open `MuseumAfterDark.uproject` in Unreal Engine

### Option 2: Manual Setup (Step by Step)

**Step 1: Generate Visual Studio Project Files**
- Right-click on `MuseumAfterDark.uproject`
- Select "Generate Visual Studio project files"
- Wait for the process to complete (you should see `MuseumAfterDark.sln` appear)

**Step 2: Build the Project**

You have two options:

**Option A: Build from Visual Studio (Recommended)**
- Open `MuseumAfterDark.sln` in Visual Studio 2022
- Set the build configuration to **"Development Editor"** (dropdown at top)
- Right-click on "MuseumAfterDarkEditor" project → Build
- Wait for compilation to complete (5-15 minutes)
- Open `MuseumAfterDark.uproject` in Unreal Engine

**Option B: Build from Command Line**
- Run `QuickBuild.bat` from the `MuseumAfterDark/` folder
- Wait for compilation to complete
- Open `MuseumAfterDark.uproject` in Unreal Engine

**Option C: Let Unreal Engine Build It**
- Double-click `MuseumAfterDark.uproject`
- When Unreal Engine prompts you to rebuild modules, click **"Yes"**
- Wait for compilation to complete (may take longer than manual build)

## Troubleshooting

### "Project needs to be compiled from source manually"

This error occurs when the C++ modules haven't been compiled yet. **You MUST build the project before opening it.**

**Solution:**
1. Run `BuildProject.bat` - this will generate project files AND build the project
2. OR follow Option 2 above to build manually
3. Make sure the `Binaries/Win64/` folder contains `.dll` files before opening the project

### "Incompatible or missing module: MuseumAfterDark"

This means the project wasn't built. The `Binaries/` folder is empty. You need to compile the C++ code first.

**Solution:**
- Run `BuildProject.bat` to build everything
- OR build from Visual Studio (see Option 2 above)

### "Cannot find Unreal Engine installation"

- Make sure Unreal Engine 5.6 is installed via Epic Games Launcher
- The project file (`MuseumAfterDark.uproject`) should automatically detect your engine installation
- If not, right-click the `.uproject` file → "Switch Unreal Engine version" → Select UE 5.6

### Build Errors

- Ensure Visual Studio has the "Game development with C++" workload installed
- Make sure you have the correct Windows SDK installed
- Try deleting the `Intermediate` and `Binaries` folders, then regenerate project files

### Project Files Not Generating

- Make sure Unreal Engine 5.6 is installed
- Try running `SetupProject.bat` as administrator
- Check that the `.uproject` file is not corrupted

## Project Structure

```
MuseumAfterDark/
├── Content/          # Blueprints, assets, maps, etc.
├── Source/           # C++ source code
├── Config/           # Project configuration files
└── MuseumAfterDark.uproject  # Project file (DO NOT DELETE)
```

## Important Notes

- **Never commit** `Binaries/`, `Intermediate/`, `Saved/`, or `.sln` files to Git
- The `.gitignore` file is already configured correctly
- If `.sln` is currently tracked in Git, run `RemoveSLNFromGit.bat` once to remove it
- Always regenerate project files if you pull changes that modify C++ code
- If you add new C++ classes, regenerate project files before committing

## Fixing Git Repository (If .sln is Already Committed)

If the `.sln` file was accidentally committed to Git before the `.gitignore` rule was added:

1. Run `RemoveSLNFromGit.bat` to remove it from Git tracking
2. Commit the change: `git commit -m "Remove .sln from tracking"`
3. Push the change: `git push`

The `.sln` file will remain on your local machine but won't be tracked by Git anymore.

## Contributing

1. Pull the latest changes
2. Regenerate project files if C++ code was modified
3. Make your changes
4. Test thoroughly
5. Commit and push

---

**Need Help?** If you encounter issues, make sure you've followed all setup steps and have the required software installed.

