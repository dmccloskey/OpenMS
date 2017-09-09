// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2017.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Douglas McCloskey $
// $Authors: Douglas McCloskey $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitationMethod.h>

//Kernal classes
#include <OpenMS/KERNEL/StandardTypes.h>

//Analysis classes
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationDescription.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModel.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModelLinear.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModelBSpline.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModelInterpolated.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModelLowess.h>

#include <cstddef> // for size_t & ptrdiff_t
#include <vector>
#include <string>

namespace OpenMS
{
    
  AbsoluteQuantitationMethod::AbsoluteQuantitationMethod() :
  DefaultParamHandler("AbsoluteQuantitationMethod")
  {
    //todo:  see MRMTransitionGroupPicker.cpp
  }
  
  AbsoluteQuantitationMethod::~AbsoluteQuantitationMethod()
  {
  }
  
  void AbsoluteQuantitationMethod::setLOD(const double& llod, const double& ulod)
  {
    llod_ = llod;
    ulod_ = ulod;
  }
  
  void AbsoluteQuantitationMethod::getLOD(double& llod, double& ulod)
  {
    llod = llod_;
    ulod = ulod_;
  }
  
  void AbsoluteQuantitationMethod::setLOQ(const double& lloq, const double& uloq)
  {
    lloq_ = lloq;
    uloq_ = uloq;
  }
  
  void AbsoluteQuantitationMethod::getLOQ(double& lloq, double& uloq)
  {
    lloq = lloq_;
    uloq = uloq_;
  }

  bool AbsoluteQuantitationMethod::checkLOD(const double & value)
  {
    bool bracketted = false;
    if (value <= ulod_ && value >= llod_)
    {
      bracketted = true;
    }
    return bracketted;
  }

  bool AbsoluteQuantitationMethod::checkLOQ(const double & value)
  {
    bool bracketted = false;
    if (value <= uloq_ && value >= lloq_)
    {
      bracketted = true;
    }
    return bracketted;
  }

  Param AbsoluteQuantitationMethod::fitTransformationModel(const std::string & transformation_model,
    TransformationModel::DataPoints& data,
    Param& transformation_model_params)
  {
    Param params;
    if (transformation_model == "TransformationModelLinear")
    {
      TransformationModelLinear tm(data, transformation_model_params);
      params = tm.getParameters();
    }
    else if (transformation_model == "TransformationModelBSpline")
    {
      TransformationModelBSpline tm(data, transformation_model_params);
      params = tm.getParameters();
    }
    else if (transformation_model == "TransformationModelInterpolated")
    {
      TransformationModelInterpolated tm(data, transformation_model_params);
      params = tm.getParameters();
    }
    else if (transformation_model == "TransformationModelLowess")
    {
      TransformationModelLowess tm(data, transformation_model_params);
      params = tm.getParameters();
    }
    else
    {
      LOG_INFO << "TransformationModel " << transformation_model << " is not supported.";
      LOG_INFO << "default TransformationModel will be used.";
      TransformationModel tm(data, transformation_model_params);
      params = tm.getParameters();
    }
    return params;
  }
  
  double AbsoluteQuantitationMethod::evaluateTransformationModel(const std::string & transformation_model,
    const double& datum,
    Param& transformation_model_params)
  {
    double result = 0.0;
    TransformationModel::DataPoints data;
    if (transformation_model == "TransformationModelLinear")
    {
      TransformationModelLinear tm(data, transformation_model_params);
      result = tm.evaluate(datum);
    }
    else if (transformation_model == "TransformationModelBSpline")
    {
      TransformationModelBSpline tm(data, transformation_model_params);
      result = tm.evaluate(datum);
    }
    else if (transformation_model == "TransformationModelInterpolated")
    {
      TransformationModelInterpolated tm(data, transformation_model_params);
      result = tm.evaluate(datum);
    }
    else if (transformation_model == "TransformationModelLowess")
    {
      TransformationModelLowess tm(data, transformation_model_params);
      result = tm.evaluate(datum);
    }
    else
    {
      LOG_INFO << "TransformationModel " << transformation_model << " is not supported.";
      LOG_INFO << "0.0 will be returned.";
    }
    return result;
  }

} // namespace

