#include "SATrachTube.h"

// Static member initialization
bool SATrachTube::isInitialized = false;

// Initialize static maps - these will be populated in the implementation
QMap<double, double> SATrachTube::BivonaStdOD;
QMap<double, double> SATrachTube::ShileyStdOD;
QMap<double, int> SATrachTube::BivonaStdDistalShaftLenNeo;
QMap<double, int> SATrachTube::ShileyStdDistalShaftLenNeo;
QMap<double, int> SATrachTube::BivonaStdDistalShaftLenPed;
QMap<double, int> SATrachTube::ShileyStdDistalShaftLenPed;
QMap<double, int> SATrachTube::ShileyStdDistalShaftLenPedExtraLong;
QMap<double, int> SATrachTube::BivonaStdDistalShaftLenPedPlus;
QMap<double, int> SATrachTube::BivonaStdProximalShaftLen;
QMap<double, int> SATrachTube::StdSuctCathSize;
QMap<QString, QString> SATrachTube::StdETTSuctDepth;

// Control Display Items
const QStringList SATrachTube::BivonaTrachTypes = {
    "Bivona - Neonatal",
    "Bivona - Pediatric",
    "Bivona - Neonatal - Flextend",
    "Bivona - Pediatric - Flextend",
    "Bivona - Pediatric Plus - Flextend",
    "Bivona - Custom"
};

const QStringList SATrachTube::ShileyTrachTypes = {
    "Shiley - Neonatal",
    "Shiley - Pediatric",
    "Shiley - Pediatric - Extra Long",
    "Shiley - Custom"
};

const QStringList SATrachTube::TrachSizes = {
    "2.5", "3.0", "3.5", "4.0", "4.5", "5.0", "5.5"
};

const QStringList SATrachTube::BivNeoTrachSizes = {
    "2.5", "3.0", "3.5", "4.0"
};

const QStringList SATrachTube::BivPedTrachSizes = {
    "2.5", "3.0", "3.5", "4.0", "4.5", "5.0", "5.5"
};

const QStringList SATrachTube::ShiNeoTrachSizes = {
    "2.5", "3.0", "3.5", "4.0", "4.5"
};

const QStringList SATrachTube::ShiPedTrachSizes = {
    "2.5", "3.0", "3.5", "4.0", "4.5", "5.0", "5.5"
};

const QStringList SATrachTube::BivPedPlusTrachSizes = {
    "4.0", "4.5", "5.0", "5.5"
};

const QStringList SATrachTube::ShiPedPExtraLongTrachSizes = {
    "5.0", "5.5"
};

const QStringList SATrachTube::BivonaCuffTypes = {
    "Cuffless", "TTS", "Air", "Foam"
};

const QStringList SATrachTube::ShileyCuffTypes = {
    "Cuffless", "Cuffed"
};

const QStringList SATrachTube::GenericFacePlateTypes = {
    "Straight", "Angled"
};

const QStringList SATrachTube::GenericCuffTypes = {
    "Cuffless", "Cuffed"
};

void SATrachTube::initializeMaps()
{
    if (isInitialized) return;

    // Standard outer diameters - Bivona
    BivonaStdOD[2.5] = 4.0;
    BivonaStdOD[3.0] = 4.7;
    BivonaStdOD[3.5] = 5.3;
    BivonaStdOD[4.0] = 6.0;
    BivonaStdOD[4.5] = 6.7; // Ped only
    BivonaStdOD[5.0] = 7.3; // Ped only
    BivonaStdOD[5.5] = 8.0; // Ped only

    // Standard outer diameters - Shiley
    ShileyStdOD[2.5] = 4.2;
    ShileyStdOD[3.0] = 4.8;
    ShileyStdOD[3.5] = 5.4;
    ShileyStdOD[4.0] = 6.0;
    ShileyStdOD[4.5] = 6.7;
    ShileyStdOD[5.0] = 7.3; // Ped only
    ShileyStdOD[5.5] = 7.9; // Ped only

    // Standard distal shaft lengths for neo tubes - Bivona
    BivonaStdDistalShaftLenNeo[2.5] = 30;
    BivonaStdDistalShaftLenNeo[3.0] = 32;
    BivonaStdDistalShaftLenNeo[3.5] = 34;
    BivonaStdDistalShaftLenNeo[4.0] = 36;

    // Standard distal shaft lengths for neo tubes - Shiley
    ShileyStdDistalShaftLenNeo[2.5] = 30;
    ShileyStdDistalShaftLenNeo[3.0] = 30;
    ShileyStdDistalShaftLenNeo[3.5] = 32;
    ShileyStdDistalShaftLenNeo[4.0] = 34;
    ShileyStdDistalShaftLenNeo[4.5] = 36;

    // Standard distal shaft lengths for ped tubes - Bivona
    BivonaStdDistalShaftLenPed[2.5] = 38;
    BivonaStdDistalShaftLenPed[3.0] = 39;
    BivonaStdDistalShaftLenPed[3.5] = 40;
    BivonaStdDistalShaftLenPed[4.0] = 41;
    BivonaStdDistalShaftLenPed[4.5] = 42;
    BivonaStdDistalShaftLenPed[5.0] = 44;
    BivonaStdDistalShaftLenPed[5.5] = 46;

    // Standard distal shaft lengths for ped tubes - Shiley
    ShileyStdDistalShaftLenPed[2.5] = 39;
    ShileyStdDistalShaftLenPed[3.0] = 39;
    ShileyStdDistalShaftLenPed[3.5] = 40;
    ShileyStdDistalShaftLenPed[4.0] = 41;
    ShileyStdDistalShaftLenPed[4.5] = 42;
    ShileyStdDistalShaftLenPed[5.0] = 44;
    ShileyStdDistalShaftLenPed[5.5] = 46;

    // Standard proximal shaft lengths for flextend tubes - Bivona only
    BivonaStdProximalShaftLen[2.5] = 20;
    BivonaStdProximalShaftLen[3.0] = 20;
    BivonaStdProximalShaftLen[3.5] = 20;
    BivonaStdProximalShaftLen[4.0] = 20;
    BivonaStdProximalShaftLen[4.5] = 30;
    BivonaStdProximalShaftLen[5.0] = 30;
    BivonaStdProximalShaftLen[5.5] = 30;

    // Standard distal shaft lengths for ped plus tubes - Bivona
    BivonaStdDistalShaftLenPedPlus[4.0] = 44;
    BivonaStdDistalShaftLenPedPlus[4.5] = 48;
    BivonaStdDistalShaftLenPedPlus[5.0] = 50;
    BivonaStdDistalShaftLenPedPlus[5.5] = 52;

    // Standard distal shaft lengths for ped tubes extra long - Shiley
    ShileyStdDistalShaftLenPedExtraLong[5.0] = 50;
    ShileyStdDistalShaftLenPedExtraLong[5.5] = 52;

    // Standard suction catheter sizes (Fr)
    StdSuctCathSize[2.5] = 6;
    StdSuctCathSize[3.0] = 6;
    StdSuctCathSize[3.5] = 8;
    StdSuctCathSize[4.0] = 8;
    StdSuctCathSize[4.5] = 10;
    StdSuctCathSize[5.0] = 10;
    StdSuctCathSize[5.5] = 10;

    // ETT suction depths
    StdETTSuctDepth["2.0"] = "14";
    StdETTSuctDepth["2.5"] = "15";
    StdETTSuctDepth["3.0"] = "16";
    StdETTSuctDepth["3.5"] = "18";
    StdETTSuctDepth["4.0"] = "20";
    StdETTSuctDepth["4.5"] = "22";
    StdETTSuctDepth["5.0"] = "24";
    StdETTSuctDepth["5.5"] = "26";

    isInitialized = true;
}

double SATrachTube::calculateStdCathInsertDepth(int proximalShaftLen, int distalShaftLen,
                                              double suctTipExtend, int adapterLen)
{
    // Ensure maps are initialized
    if (!isInitialized) {
        initializeMaps();
    }

    // Calculates the recommended suction catheter insertion depth from trach tube parameters, in cm
    return (proximalShaftLen + adapterLen + distalShaftLen + suctTipExtend) / 10.0;
}