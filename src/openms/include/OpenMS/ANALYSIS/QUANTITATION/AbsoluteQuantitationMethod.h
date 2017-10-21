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
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/KERNEL/MRMFeature.h>

//Analysis classes
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModel.h>

#include <cstddef> // for size_t & ptrdiff_t
#include <vector>
#include <string>

namespace OpenMS
{

  /**
    @brief AbsoluteQuantitationMethod is a class to handle the relationship between
    runs, components, and actual concentrations.
  */
  class OPENMS_DLLAPI AbsoluteQuantitationMethod
  {

public:    
  
  //@{
  /// Constructor
  AbsoluteQuantitationMethod();

  /// Destructor
  ~AbsoluteQuantitationMethod();
  //@}

  /// LLOD and ULOD setter
  void setLOD(const double& llod, const double& ulod);

  /// LLOD and ULOD getter
  void getLOD(double& llod, double& ulod);
  
  /// LLOQ and ULOQ setter
  void setLOQ(const double& lloq, const double& uloq);

  /// LLOQ and ULOQ getter
  void getLOQ(double& lloq, double& uloq);

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

  /// component_name, IS_name, and feature_name setter
  void setComponentISFeatureNames(const String& component_name, const String& IS_name, const String& feature_name);

  /// component_name, IS_name, and feature_name getter
  void getComponentISFeatureNames(String& component_name, String& IS_name, String& feature_name);
  
  /// concentration_units setter
  void setConcentrationUnits(const String& concentration_units);

  /// concentration_units getter
  void getConcentrationUnits(String& concentration_units);
  
  /// transformation_model and transformation_model_params setter
  void setTransformationModel(const String& transformation_model, const Param& transformation_model_params);

  /// transformation_model and transformation_model_params getter
  void getTransformationModel(String& transformation_model, Param& transformation_model_params);

  /// actual concentration setter
  void setActualConcentration(const double& actual_concentration);
  
  /// actual concentration getter
  void getActualConcentration(double& actual_concentration);
  
  /// statistics setter
  void setStatistics(const int& n_points, const double& correlation_coefficient);
  
  /// statistics getter
  void getStatistics(int& n_points, double& correlation_coefficient);
  
  // /// feature_name setter
  // void setFeatureName(const String& feature_name);

  // /// feature_name getter
  // void getFeatureName(String& feature_name);

  /**
  @brief This function fits the transformation model with the data
    and parameters

  @param transformation_model name of the transformation model
  @param data data to fit to the model
  @param transformation_model_params model parameters

  @return updated parameters.
  */ 
  Param fitTransformationModel(const String & transformation_model,
    const TransformationModel::DataPoints& data,
    const Param& transformation_model_params);

  /**
  @brief This function inverts and evaluates the transformation model
    with the empty data and fitted parameters

  @param transformation_model name of the transformation model
  @param datum datum to evaluate the model at
  @param transformation_model_params model parameters

  @return evaluated datum.
  */ 
  double evaluateTransformationModel(const String & transformation_model,
    const double& datum,
    const Param& transformation_model_params);
                                  
  // members

  /// id of the component
  String component_name_;
  
  /// name of the feature (i.e., peak_apex_int or peak_area)
  String feature_name_;

  /// lower limit of detection (LLOD) of the transition
  double llod_;

  /// lower limit of quantitation (LLOQ) of the transition
  double lloq_;

  /// upper limit of detection (LLOD) of the transition
  double ulod_;

  /// upper limit of quantitation (LLOQ) of the transition
  double uloq_;

  /// number of points used in a calibration curve
  int n_points_;

  /// the Pearson R value for the correlation coefficient of the calibration curve
  double correlation_coefficient_;

  /// the internal standard (IS) name for the transition
  String IS_name_;

  /// the known concentration of the component
  double actual_concentration_;

  /// concentration units of the component's concentration
  String concentration_units_;
  
  /// transformation model
  String transformation_model_;

  /// transformation model parameters
  Param transformation_model_params_;  
  };

}
#endif // OPENMS_ANALYSIS_OPENSWATH_ABSOLUTEQUANTIFICATIONMETHOD_H
