/******************************************************************************
 *
 * Project:  GDAL
 * Purpose:  Arrow Database Connectivity driver
 * Author:   Even Rouault, <even dot rouault at spatialys.com>
 *
 ******************************************************************************
 * Copyright (c) 2024, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#include "ogrsf_frmts.h"
#include "ogradbcdrivercore.h"

/************************************************************************/
/*                             IsDuckDB()                               */
/************************************************************************/

bool OGRADBCDriverIsDuckDB(const GDALOpenInfo *poOpenInfo)
{
    return poOpenInfo->nHeaderBytes >= 20 &&
           memcmp(poOpenInfo->pabyHeader + 8, "DUCK\x40\0\0\0\0\0\0\0",
                  4 + 8) == 0;
}

/************************************************************************/
/*                             IsSQLite3()                              */
/************************************************************************/

bool OGRADBCDriverIsSQLite3(const GDALOpenInfo *poOpenInfo)
{
    constexpr const char *SQLITE3_SIGNATURE = "SQLite format 3";
    return poOpenInfo->nHeaderBytes >= 512 &&
           memcmp(poOpenInfo->pabyHeader, SQLITE3_SIGNATURE,
                  strlen(SQLITE3_SIGNATURE)) == 0;
}

/************************************************************************/
/*                             IsParquet()                              */
/************************************************************************/

bool OGRADBCDriverIsParquet(const GDALOpenInfo *poOpenInfo)
{
    constexpr const char *PARQUET_SIGNATURE = "PAR1";
    return poOpenInfo->nHeaderBytes >=
               static_cast<int>(strlen(PARQUET_SIGNATURE)) &&
           memcmp(poOpenInfo->pabyHeader, PARQUET_SIGNATURE,
                  strlen(PARQUET_SIGNATURE)) == 0;
}

/************************************************************************/
/*                    OGRADBCDriverIdentify()                           */
/************************************************************************/

int OGRADBCDriverIdentify(GDALOpenInfo *poOpenInfo)
{
    return (STARTS_WITH(poOpenInfo->pszFilename, "ADBC:") ||
            OGRADBCDriverIsDuckDB(poOpenInfo) ||
            OGRADBCDriverIsSQLite3(poOpenInfo) ||
            OGRADBCDriverIsParquet(poOpenInfo)) &&
           !STARTS_WITH(poOpenInfo->pszFilename, "/vsi") &&
           !EQUAL(CPLGetExtension(poOpenInfo->pszFilename), "mbtiles");
}

/************************************************************************/
/*                  OGRADBCDriverSetCommonMetadata()                    */
/************************************************************************/

void OGRADBCDriverSetCommonMetadata(GDALDriver *poDriver)
{
    poDriver->SetDescription(DRIVER_NAME);
    poDriver->SetMetadataItem(GDAL_DCAP_VECTOR, "YES");
    poDriver->SetMetadataItem(GDAL_DMD_LONGNAME, "Arrow Database Connectivity");
    poDriver->SetMetadataItem(GDAL_DMD_HELPTOPIC, "drivers/vector/adbc.html");

    poDriver->SetMetadataItem(GDAL_DMD_CONNECTION_PREFIX, "ADBC:");

    poDriver->SetMetadataItem(
        GDAL_DMD_OPENOPTIONLIST,
        "<OpenOptionList>"
        "  <Option name='ADBC_DRIVER' type='string' description='ADBC driver "
        "name'/>"
        "  <Option name='SQL' type='string' "
        "description='SQL statement from which to build layer'/>"
        "  <Option name='ADBC_OPTION_*' type='string' "
        "description='Option to pass to AdbcDatabaseSetOption()'/>"
        "</OpenOptionList>");
    poDriver->SetMetadataItem(GDAL_DMD_SUPPORTED_SQL_DIALECTS,
                              "NATIVE OGRSQL SQLITE");

    poDriver->SetMetadataItem(GDAL_DCAP_OPEN, "YES");
    poDriver->pfnIdentify = OGRADBCDriverIdentify;
}

/************************************************************************/
/*                   DeclareDeferredOGRADBCPlugin()                     */
/************************************************************************/

#ifdef PLUGIN_FILENAME
void DeclareDeferredOGRADBCPlugin()
{
    if (GDALGetDriverByName(DRIVER_NAME) != nullptr)
    {
        return;
    }
    auto poDriver = new GDALPluginDriverProxy(PLUGIN_FILENAME);
#ifdef PLUGIN_INSTALLATION_MESSAGE
    poDriver->SetMetadataItem(GDAL_DMD_PLUGIN_INSTALLATION_MESSAGE,
                              PLUGIN_INSTALLATION_MESSAGE);
#endif
    OGRADBCDriverSetCommonMetadata(poDriver);
    GetGDALDriverManager()->DeclareDeferredPluginDriver(poDriver);
}
#endif
