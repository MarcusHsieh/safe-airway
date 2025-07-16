# Safe Airway Application Workflow

Workflow of the Safe Airway Qt C++ application.

## Current Application Structure

```mermaid
graph TD
    A[Application Start] --> B[Main Window - Home Page]
    B --> C{Select Case Type}
    C --> D[Tracheostomy]
    C --> F[Difficult Airway]
    C --> G[Laryngotracheal Reconstruction]
    
    D --> H[Tracheostomy Display Page]
    F --> J[Difficult Airway Display Page]
    G --> K[LTR Display Page]
    
    H --> L[Patient Information Input]
    J --> L
    K --> L
    
    L --> M[Trach Specifications Table]
    M --> N[Emergency Scenarios Panel]
    
    N --> O{Emergency Type}
    O --> P[Can't Suction]
    O --> Q[Can't Ventilate]
    O --> R[O2 Sat Drop]
    O --> S[Decannulation]
    O --> T[Hemoptysis]
    
    P --> U[Emergency Instructions Display]
    Q --> U
    R --> U
    S --> U
    T --> U
    
    U --> V[Save & Exit or Cancel]
    V --> B
```

## Enhanced Workflow with New Features

```mermaid
graph TD
    A[Application Start] --> NEW1[New: Case Selection Start Page]
    NEW1 --> NEW2{Load Existing or Create New?}
    
    NEW2 --> NEW3[Load Existing Case]
    NEW2 --> B[Create New Case - Home Page]
    
    NEW3 --> NEW4[Display Saved Case Data]
    NEW4 --> H
    
    B --> C{Select Case Type}
    C --> D[Tracheostomy]
    C --> F[Difficult Airway]
    C --> G[Laryngotracheal Reconstruction]
    
    D --> H[Tracheostomy Display Page - Enhanced]
    F --> J[Difficult Airway Display Page - Enhanced]
    G --> K[LTR Display Page - Enhanced]
    
    H --> L[Patient Information Input - Larger Text]
    J --> L
    K --> L
    
    L --> M[Enhanced: Expandable Trach Table]
    M --> N[Emergency Scenarios Panel]
    M --> NEW5[New: Freeze/Print Mode Toggle]
    
    NEW5 --> NEW6{Freeze Mode Active?}
    NEW6 --> NEW7[Interface Locked - Emergency Only]
    NEW6 --> N[Normal Mode - Full Access]
    
    N --> O{Emergency Type}
    O --> P[Can't Suction]
    O --> Q[Enhanced: Can't Ventilate - Synced ETT Size]
    O --> R[O2 Sat Drop]
    O --> S[Enhanced: Decannulation - Synced ETT Size]
    O --> T[Hemoptysis]
    
    Q --> NEW8[ETT Size matches Suction Size]
    S --> NEW8
    P --> U[Emergency Instructions Display]
    NEW8 --> U
    R --> U
    T --> U
    
    NEW7 --> O
    
    U --> V[Save & Exit or Cancel]
    V --> NEW1
```

## Key Components

### 1. Main Application Entry
- **File**: `main.cpp`
- **Purpose**: Application initialization and main window setup

### 2. Main Window Controller
- **Files**: `mainwindow.h`, `mainwindow.cpp`
- **Purpose**: Core application logic, emergency scenario handling, UI management

### 3. User Interface
- **File**: `mainwindow.ui`
- **Purpose**: Qt Designer UI layout with stacked widgets for different pages

### 4. Emergency Scenarios
- **Current Types**: Can't Suction, Can't Ventilate, O2 Sat Drop, Decannulation, Hemoptysis
- **Enhancement**: ETT size synchronization with suction size for specific scenarios

## New Features to Implement

### 1. Start Page for Case Selection
- Add new page before main case type selection
- Options: "Load Existing Case" and "Create New Case"
- File management for saved cases

### 2. Enhanced Text Size
- Increase font sizes throughout application
- Optimize for 11.3" x 8.2" screen dimensions

### 3. Expandable Table
- Modify table to fill horizontal space instead of truncating
- Implement proper column sizing and content wrapping

### 4. ETT Size Synchronization
- Link "6.0 endotracheal tube" text in emergency scenarios with suction size field
- Apply to "Can't Ventilate" and "Decannulation" scenarios

### 5. Freeze/Print Mode
- Add toggle to lock interface except emergency scenarios
- Prevent patient interference with critical data
- Maintain emergency scenario accessibility

## Technical Implementation Notes

- **Framework**: Qt 6 with C++17
- **UI Design**: Qt Designer (.ui files)
- **Build System**: qmake (.pro files)
- **Resource Management**: Qt Resource System (.qrc files) 