# Safe Airway - Project Overview

## Executive Summary

Safe Airway is a medical form management application designed for Nemours Children's Health, specializing in pediatric airway management procedures. It provides digital forms for critical medical procedures with emergency guidance features, ensuring safe and standardized care for patients requiring airway interventions.

## Core Purpose & Vision

### Problem Statement
Healthcare providers need a standardized, reliable system for managing critical airway procedures in pediatric patients, with immediate access to emergency protocols and patient-specific information during potentially life-threatening situations.

### Solution
A cross-platform desktop application that digitizes and standardizes airway management forms, providing:
- Color-coded procedure forms for quick identification
- Real-time emergency scenario guidance
- Secure local patient data storage
- Enhanced visibility for OR/emergency room environments

## Technical Architecture

### Technology Stack

#### Core Framework
- **Qt Framework 6.7.3**: Cross-platform C++ GUI framework
- **C++17**: Modern C++ standard for robust performance
- **CMake/qmake**: Build system management

#### Key Libraries & Components
- **Qt Core**: Application foundation and event handling
- **Qt Widgets**: Native UI components
- **Qt PrintSupport**: Medical document printing capabilities
- **JSON**: Data serialization and storage format

#### Development Tools
- **MinGW 11.2 64-bit**: Windows compiler toolchain
- **GCC 9.0+**: Linux/Unix compiler
- **Qt Creator**: Recommended IDE

### Application Architecture

```
safe-airway/
├── main.cpp                    # Entry point
├── src/
│   ├── core/                   # Core application services
│   │   ├── Application         # Singleton app manager
│   │   └── CaseManager         # Case lifecycle management
│   │
│   ├── models/                 # Data layer
│   │   ├── Case               # Case data structure
│   │   └── EmergencyScenario  # Emergency protocol models
│   │
│   ├── utils/                  # Utilities
│   │   ├── ConfigManager      # Configuration management
│   │   ├── StyleManager       # UI theming and styles
│   │   └── SATrachTube        # Medical calculations
│   │
│   ├── views/                  # Main UI screens
│   │   ├── MainWindow         # Application shell
│   │   ├── CaseSelectionView  # Case browser
│   │   ├── TracheostomyFormView
│   │   ├── NewTracheostomyFormView
│   │   ├── DifficultAirwayFormView
│   │   └── LTRFormView        # Form implementations
│   │
│   └── widgets/                # Reusable components
│       ├── BaseFormWidget     # Form base class
│       ├── EmergencyPanel     # Emergency guidance UI
│       ├── EscOverlayMenu     # Quick-access menu
│       ├── NotificationWidget # Toast notifications
│       └── PatientInfoWidget  # Patient data display
```

## Key Features

### 1. Four Specialized Medical Forms

#### Tracheostomy Form (Teal)
- Standard tracheostomy procedure documentation
- Equipment specifications tracking
- Suction depth and size calculations

#### New Tracheostomy Form (Pink)
- Initial tracheostomy placement protocols
- Enhanced safety checks for first-time procedures
- Specialized equipment requirements

#### Difficult Airway Form (Yellow)
- Complex airway management scenarios
- Multiple contingency planning options
- Risk assessment documentation

#### LTR Form (Blue)
- Laryngotracheal reconstruction procedures
- Long-term care planning
- Surgical follow-up tracking

### 2. Emergency Management System
- **Real-time Guidance**: Step-by-step emergency protocols
- **ETT Size Synchronization**: Automatic calculation of endotracheal tube sizes
- **Decision Tree Navigation**: Visual guidance for critical decisions
- **Freeze Mode**: Lock interface during procedures while maintaining emergency access

### 3. Data Management
- **Local JSON Storage**: HIPAA-compliant local data storage
- **Organized File Structure**: Cases saved by type with timestamps
- **Case Management**: Save, load, and browse historical cases
- **Recent Files List**: Quick access to frequently used cases

### 4. User Interface Design
- **Percentage-Based Layout**: Responsive across different screen sizes
- **Enhanced Visibility**: Large fonts (32-40pt) readable from 5-10 feet
- **Color-Coded System**: Instant visual procedure identification
- **Compact Header**: Space-efficient design with integrated branding
- **Modern Notifications**: Non-intrusive popup notifications

### 5. Privacy & Compliance
- **HIPAA Compliance**: Patient last names and MRNs hidden from display
- **Backend Preservation**: Full data maintained for Epic integration
- **Secure Storage**: Local-only data storage (no cloud transmission)

## Data Flow & Storage

### Storage Structure
- Each case/sign should have a unique ID
- When saving a case/sign it should overwrite it's current file instead of creating a new file
- Best choice for easiest transition into Epic database

### JSON Data Schema
```json
{
  "caseId": "string",
  "caseType": "enum",
  "timestamp": "ISO8601",
  "patient": {
    "firstName": "string",
    "lastName": "string (hidden)",
    "mrn": "string (hidden)",
    "dateOfBirth": "date"
  },
  "specifications": [...],
  "emergencyScenarios": [...],
  "decisionBox": {...}
}
```

## Use Cases & Workflows

### Primary Use Cases

1. **Pre-Operative Planning**
   - Review patient history
   - Select appropriate equipment
   - Document procedure plans

2. **Intra-Operative Documentation**
   - Real-time data entry
   - Emergency protocol access
   - Equipment tracking

3. **Emergency Response**
   - Quick access to emergency scenarios
   - Step-by-step guidance
   - Decision support tools

4. **Post-Operative Records**
   - Complete procedure documentation
   - Equipment specifications for future reference
   - Care continuation planning

## Deployment & Distribution

### Build Configurations
- **Development**: Debug build with verbose logging
- **Production**: Optimized release build with Qt dependency bundling
- **Distribution**: Self-contained package (~25-40MB) with all dependencies

### Platform Support
- **Windows 10/11**: Primary deployment target
- **Linux/WSL**: Development and testing environment
- **Cross-platform**: Qt ensures future portability

## Future Roadmap

### Near-term Enhancements
- Epic EMR integration via C++ API
- Cloud backup options (with encryption)
- Multi-user collaboration features
- Advanced reporting and analytics

### Long-term Vision
- Mobile companion app for bedside access
- AI-powered decision support
- Integration with medical device APIs
- Telemedicine consultation features

## Development Guidelines

### Code Standards
- C++17 modern features
- Qt best practices and signals/slots
- RAII for resource management
- Comprehensive error handling

### Testing Strategy
- Unit tests for core logic
- UI automation testing
- Medical scenario validation
- Performance benchmarking

### Security Considerations
- No hardcoded credentials
- Secure local storage
- Input validation
- Audit logging capability

## Success Metrics

- **Reliability**: 99.9% uptime during procedures
- **Performance**: <100ms response time for all actions
- **Usability**: Readable from 5-10 feet distance
- **Compliance**: Full HIPAA compliance maintained
- **Adoption**: Used in 100% of applicable procedures

## Version History

- **v1.0.0-alpha** (Current)
  - Initial release with four form types
  - Basic emergency scenarios
  - Local storage implementation

- **v0.2** (Legacy)
  - Prototype version
  - Basic form functionality
  - Limited emergency features

## Contact & Support

**Project Lead**: Nemours Children's Health IT Department
**Repository**: [Internal Git Repository]
**Documentation**: See PROJECT_DOCUMENTATION.md for technical details
**License**: © 2024 Nemours Children's Health