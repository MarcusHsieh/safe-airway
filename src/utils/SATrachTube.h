#ifndef SATRACHTUBE_H
#define SATRACHTUBE_H

#include <QMap>
#include <QString>
#include <QStringList>

// Static class with parameters for standard and custom tracheostomy tubes
class SATrachTube
{
public:
    // Standard outer diameters
    static QMap<double, double> BivonaStdOD;
    static QMap<double, double> ShileyStdOD;

    // Standard distal shaft lengths
    static QMap<double, int> BivonaStdDistalShaftLenNeo; // neonatal tubes only
    static QMap<double, int> ShileyStdDistalShaftLenNeo; // neonatal tubes only
    static QMap<double, int> BivonaStdDistalShaftLenPed; // pediatric tubes only
    static QMap<double, int> ShileyStdDistalShaftLenPed; // pediatric tubes only
    static QMap<double, int> ShileyStdDistalShaftLenPedExtraLong; // pediatric extra long tubes only
    static QMap<double, int> BivonaStdDistalShaftLenPedPlus; // flextend pediatric plus tubes only
    static QMap<double, int> BivonaStdProximalShaftLen; // flextend tubes only

    // Standard suction catheter sizes
    static QMap<double, int> StdSuctCathSize;

    // ETT suction depths
    static QMap<QString, QString> StdETTSuctDepth;

    // Constants
    static const int AdapterLen = 25; // standard length of anesthetic adaptor
    static const int SuctTipExtend = 5; // standard recommendation for suction distance beyond cannula tip

    // Control Display Items
    static const QStringList BivonaTrachTypes;
    static const QStringList ShileyTrachTypes;
    static const QStringList TrachSizes;
    static const QStringList BivNeoTrachSizes;
    static const QStringList BivPedTrachSizes;
    static const QStringList ShiNeoTrachSizes;
    static const QStringList ShiPedTrachSizes;
    static const QStringList BivPedPlusTrachSizes;
    static const QStringList ShiPedPExtraLongTrachSizes;
    static const QStringList BivonaCuffTypes;
    static const QStringList ShileyCuffTypes;
    static const QStringList GenericFacePlateTypes;
    static const QStringList GenericCuffTypes;

    // Calculation functions
    static double calculateStdCathInsertDepth(int proximalShaftLen, int distalShaftLen,
                                            double suctTipExtend = SuctTipExtend,
                                            int adapterLen = AdapterLen);

private:
    // Private constructor to prevent instantiation
    SATrachTube() = delete;

    // Static initialization helper
    static void initializeMaps();
    static bool isInitialized;
};

#endif // SATRACHTUBE_H