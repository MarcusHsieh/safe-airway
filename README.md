# Safe Airway Application

A digital form application for airway management at Nemours Children's Health, built with Qt 6 and C++17.

## Quick Start

### Prerequisites
- Qt 6.7.3 with MinGW 11.2 64-bit (or Qt 6.5.9 LTS)
- C++17 compatible compiler
- CMake or qmake (included with Qt)

### Building

#### Using qmake (recommended)
```bash
# Set up Qt environment
export QTDIR=/path/to/Qt/6.7.3/mingw_64
export PATH=$QTDIR/bin:$PATH

# Build the application
qmake safe-airway.pro
make -j4

# Run the application
./release/safe-airway
```

#### Using Qt Creator
1. Open `safe-airway.pro` in Qt Creator
2. Select the kit: Desktop Qt 6.7.3 MinGW 11.2 64-bit
3. Configure Project → Build → Run

### Features
- **Four Color-Coded Forms**: Tracheostomy (Teal), New Tracheostomy (Pink), Difficult Airway (Yellow), LTR (Blue)
- **Local JSON Storage**: Cases saved to `~/Documents/SafeAirway/case_saves/`
- **Emergency Scenarios**: Interactive emergency guidance with automatic ETT size synchronization
- **Freeze Mode**: Lock interface during procedures while keeping emergency scenarios accessible
- **Enhanced UI**: Optimized for 11.3" x 8.2" screens with large fonts and expandable tables
- **Case Management**: Save, load, and manage cases with recent files list

### Application Structure
- `src/core/` - Application services (CaseManager, Application)
- `src/models/` - Data models (Case, EmergencyScenario)
- `src/utils/` - Utilities (ConfigManager, StyleManager)
- `src/views/` - Main windows and form views
- `src/widgets/` - Reusable UI components

### Data Storage
Cases are automatically saved to:
```
~/Documents/SafeAirway/case_saves/
├── tracheostomy/
├── new_tracheostomy/
├── difficult_airway/
└── ltr/
```

Each case is saved as a JSON file with timestamp and case ID.

## Documentation
See `PROJECT_DOCUMENTATION.md` for comprehensive technical documentation.
See `workflow-diagram.md` for application workflow details.

## Version
Current version: 1.0.0-alpha

## License
© 2024 Nemours Children's Health