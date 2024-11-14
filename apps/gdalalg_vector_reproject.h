/******************************************************************************
 *
 * Project:  GDAL
 * Purpose:  "reproject" step of "vector pipeline"
 * Author:   Even Rouault <even dot rouault at spatialys.com>
 *
 ******************************************************************************
 * Copyright (c) 2024, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#ifndef GDALALG_VECTOR_REPROJECT_INCLUDED
#define GDALALG_VECTOR_REPROJECT_INCLUDED

#include "gdalalg_vector_pipeline.h"

//! @cond Doxygen_Suppress

/************************************************************************/
/*                    GDALVectorReprojectAlgorithm                      */
/************************************************************************/

class GDALVectorReprojectAlgorithm final
    : public GDALVectorPipelineStepAlgorithm
{
  public:
    static constexpr const char *NAME = "reproject";
    static constexpr const char *DESCRIPTION = "Reproject.";
    static constexpr const char *HELP_URL = "";  // TODO

    static std::vector<std::string> GetAliases()
    {
        return {};
    }

    GDALVectorReprojectAlgorithm();

  private:
    bool RunImpl(GDALProgressFunc pfnProgress, void *pProgressData) override;

    std::string m_srsCrs{};
    std::string m_dstCrs{};
};

//! @endcond

#endif /* GDALALG_VECTOR_REPROJECT_INCLUDED */
