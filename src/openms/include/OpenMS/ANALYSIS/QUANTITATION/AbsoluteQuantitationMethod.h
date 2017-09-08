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

#ifndef OPENMS_ANALYSIS_OPENSWATH_ABSOLUTEQUANTIFICATIONMETHOD_H
#define OPENMS_ANALYSIS_OPENSWATH_ABSOLUTEQUANTIFICATIONMETHOD_H

#include <OpenMS/config.h>

//Kernal classes
#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/KERNEL/MRMFeature.h>

//Analysis classes
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationDescription.h>

#include <cstddef> // for size_t & ptrdiff_t
#include <vector>
#include <string>

namespace OpenMS
{

  /**
    @brief AbsoluteQuantitationMethod is a class to handle the relationship between
    runs, components, and actual concentrations.
  */
  class OPENMS_DLLAPI AbsoluteQuantitationMethod :
  public DefaultParamHandler
  {

public:    
  
  //@{
  /// Constructor
  AbsoluteQuantitationMethod();

  /// Destructor
  ~AbsoluteQuantitationMethod();
  //@}

  /**
  @brief This function checks if the value is within the
    limits of detection (LOD)

  */ 
  bool checkLOD(const double & value);

  /**
  @brief This function checks if the value is within the
    limits of quantitation (LOQ)

  */ 
  bool checkLOQ(const double & value);

  /**
  @brief This function returns the transformation model
    with parameters applied

  */ 
  auto getTransformationModel(const std::string & transformation_model);
                                  
  // members

  /// id of the component
  std::string component_id_;
  
  /// name of the feature (i.e., peak_apex_int or peak_area)
  std::string feature_name_;

  /// lower limit of detection (LLOD) of the transition
  double llod_;

  /// lower limit of quantitation (LLOQ) of the transition
  double lloq_;

  /// upper limit of detection (LLOD) of the transition
  double ulod_;

  /// upper limit of quantitation (LLOQ) of the transition
  double uloq_;

  /// number of points used in a calibration curve
  double n_points_;

  /// the Pearson R value for the correlation coefficient of the calibration curve
  double correlation_coefficient_;

  /// the internal standard (IS) name for the transition
  String IS_name_;

  /// the known concentration of the component
  double actual_concentration_;

  /// concentration units of the component's concentration
  std::string concentration_units_;

  /// transformation model
  std::string transformation_model_;

  /// transformation model parameters
  std::string transformation_model_params_;  

  };

}
#endif // OPENMS_ANALYSIS_OPENSWATH_ABSOLUTEQUANTIFICATIONMETHOD_H

