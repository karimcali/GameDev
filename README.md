# MuseumAfterDark - Unreal Engine Project

## Prerequisites

- **Unreal Engine 5.6** (or compatible version)
- **Visual Studio 2022** (or Visual Studio 2019/2022 with C++ development tools)
- **Windows 10/11** (64-bit)

## Initial Setup (After Downloading from GitHub)

This project uses both **C++ and Blueprints**. When you download the project from GitHub, you need to regenerate the project files and compile the C++ code before opening it in Unreal Engine.

### Option 1: Automatic Setup (Recommended)

1. Navigate to the project folder: `MuseumAfterDark/`
2. Double-click `SetupProject.bat` (or right-click and select "Run as administrator")
3. Wait for the project files to be generated
4. Open `MuseumAfterDark.uproject` - Unreal Engine will prompt you to rebuild modules, click "Yes"

### Option 2: Manual Setup

1. **Generate Visual Studio Project Files:**
   - Right-click on `MuseumAfterDark.uproject`
   - Select "Generate Visual Studio project files"
   - Wait for the process to complete

2. **Open and Build the Project:**
   - Double-click `MuseumAfterDark.uproject`
   - When Unreal Engine prompts you to rebuild modules, click **"Yes"**
   - The project will compile automatically

   **OR** (if the above doesn't work):
   - Open `MuseumAfterDark.sln` in Visual Studio
   - Set the build configuration to **"Development Editor"**
   - Build the solution (Build → Build Solution or F7)
   - Once compilation completes, open `MuseumAfterDark.uproject` in Unreal Engine

## Troubleshooting

### "Project needs to be compiled from source manually"

This error occurs when the C++ modules haven't been compiled yet. Follow the setup steps above.

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

