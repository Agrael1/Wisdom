# Wisdom Version Management

This project uses automated semantic versioning with a single source of truth.

## Version File

The project version is stored in the `VERSION` file at the repository root. This file contains a single line with the version number in the format `MAJOR.MINOR.PATCH` (e.g., `0.7.0`).

## Version Components

- **MAJOR**: Breaking changes or significant new features
- **MINOR**: New features that are backward compatible
- **PATCH**: Bug fixes and minor improvements

## Local Development

### Using Python (Cross-platform)

```bash
# Get current version
python version.py get

# Set specific version
python version.py set 1.2.3

# Bump version
python version.py major  # 1.2.3 -> 2.0.0
python version.py minor  # 1.2.3 -> 1.3.0
python version.py patch  # 1.2.3 -> 1.2.4
```

### Using PowerShell (Windows)

```powershell
# Get current version
.\version.ps1 Get

# Set specific version
.\version.ps1 Set 1.2.3

# Bump version
.\version.ps1 Major  # 1.2.3 -> 2.0.0
.\version.ps1 Minor  # 1.2.3 -> 1.3.0
.\version.ps1 Patch  # 1.2.3 -> 1.2.4
```

## GitHub Actions Automation

The repository includes a **Version Bump** workflow that can be manually triggered from the GitHub Actions tab.

### Triggering a Version Bump

1. Go to **Actions** tab in GitHub
2. Select **Version Bump** workflow
3. Click **Run workflow**
4. Choose bump type (major, minor, or patch)
5. Click **Run workflow**

The workflow will:
1. Bump the version in the `VERSION` file
2. Commit the change to the current branch
3. Create and push a git tag (e.g., `v0.7.0`)
4. Create a GitHub release with changelog reference

## Release Process

### For Regular Releases (on master)

```bash
# 1. Ensure you're on master and up to date
git checkout master
git pull origin master

# 2. Bump version locally (optional, for testing)
python version.py minor

# 3. Use GitHub Actions for official release
# Go to Actions > Version Bump > Run workflow

# 4. Or manually:
./version.py minor
git add VERSION
git commit -m "chore: bump version to $(cat VERSION)"
git push origin master
git tag -a "v$(cat VERSION)" -m "Release v$(cat VERSION)"
git push origin "v$(cat VERSION)"
```

### For Hotfixes

```bash
# Create hotfix branch from release tag
git checkout -b hotfix/fix-description v0.6.12

# Make your fixes
git add .
git commit -m "fix: description of fix"

# Bump patch version
python version.py patch

# Commit version bump
git add VERSION
git commit -m "chore: bump version to $(cat VERSION)"

# Create PR to master
git push origin hotfix/fix-description
```

## CMake Integration

CMake automatically reads the version from the `VERSION` file:

```cmake
# Version is set in CMakeLists.txt
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" WISDOM_VERSION)
project(Wisdom VERSION ${WISDOM_VERSION})
```

The version is available as:
- `${WISDOM_VERSION}` - Full version string
- `${WISDOM_VERSION_MAJOR}` - Major component
- `${WISDOM_VERSION_MINOR}` - Minor component  
- `${WISDOM_VERSION_PATCH}` - Patch component
- `${PROJECT_VERSION}` - Same as WISDOM_VERSION

## Generated Code

The version is automatically embedded in generated code files:
- `wisdom/include/wisdom/generated/api/api.hpp` - Contains version in documentation header
- Module files (`.ixx`) - Contains version in module documentation
- NuGet packages - Version is set from CMake

## Best Practices

1. **Never manually edit version** in CMakeLists.txt or generated files
2. **Always use the VERSION file** as the single source of truth
3. **Use scripts or GitHub Actions** to bump versions
4. **Create git tags** for all releases (done automatically by GitHub Actions)
5. **Update Changelog.md** before bumping version for releases
6. **Use semantic versioning** consistently:
   - `major` for breaking changes
   - `minor` for new features
   - `patch` for bug fixes

## Continuous Integration

All CI workflows use the version from the `VERSION` file automatically. No configuration changes needed when bumping versions.

## Troubleshooting

### Version mismatch in generated files

Regenerate the API:
```bash
cmake --build build --target generator
```

### Git tag already exists

Delete the local and remote tag:
```bash
git tag -d v0.7.0
git push origin :refs/tags/v0.7.0
```

### GitHub Actions permission denied

Ensure the workflow has `contents: write` permission (already configured).
