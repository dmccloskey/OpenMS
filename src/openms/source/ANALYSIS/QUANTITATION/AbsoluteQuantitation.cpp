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

#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitation.h>

//Kernal classes
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/KERNEL/MSChromatogram.h>
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/KERNEL/MRMTransitionGroup.h>
#include <OpenMS/KERNEL/MRMFeature.h>

//Analysis classes
#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModel.h>
#include <OpenMS/ANALYSIS/TARGETED/TargetedExperiment.h>

//Quantitation classes
#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitationStandards.h>
#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitationMethod.h>

//Math classes
#include <OpenMS/MATH/STATISTICS/StatisticsFunctions.h>

//Standard library
#include <cstddef> // for size_t & ptrdiff_t
#include <vector>
#include <string>
#include <math.h>

namespace OpenMS
{
  
  AbsoluteQuantitation::AbsoluteQuantitation() :
  DefaultParamHandler("AbsoluteQuantitation")
  {
    //todo:  see MRMTransitionGroupPicker.cpp
  }
  
  AbsoluteQuantitation::~AbsoluteQuantitation()
  {
  }

  void AbsoluteQuantitation::setQuantMethods(std::vector<AbsoluteQuantitationMethod>& quant_methods)
  {
    quant_methods_.clear();
    String component_name,IS_component_name,feature_name;
    for (size_t i = 0; i < quant_methods.size(); i++)
    {
      quant_methods[i].getComponentISFeatureNames(component_name,IS_component_name,feature_name);
      quant_methods_[component_name] = quant_methods[i];
    }
  }

  double AbsoluteQuantitation::calculateRatio(Feature & component_1, Feature & component_2, String & feature_name)
  {
    double ratio = 0.0;
    if (component_1.metaValueExists(feature_name) && component_2.metaValueExists(feature_name))
    {
      double feature_1 = component_1.getMetaValue(feature_name);
      double feature_2 = component_2.getMetaValue(feature_name);
      ratio = feature_1/feature_2;
    } 
    else if (component_1.metaValueExists(feature_name))
    {
      LOG_INFO << "Warning: no IS found for component " << component_1.getMetaValue("native_id") << ".";
      double feature_1 = component_1.getMetaValue(feature_name);
      ratio = feature_1;
    } 
    else
    {
      LOG_INFO << "Feature metaValue " << feature_name << " not found for components " << component_1.getMetaValue("native_id") << " and " << component_2.getMetaValue("native_id") << ".";
    }

    return ratio;
  }
  
  double AbsoluteQuantitation::calculateBias(double & actual_concentration, double & calculated_concentration)
  {
    double bias = 0.0;
    bias = fabs(actual_concentration - calculated_concentration)/actual_concentration*100;
    return bias;
  }
  
  Param AbsoluteQuantitation::fitCalibration(
    const std::vector<AbsoluteQuantitationStandards::featureConcentration> & component_concentrations,
    const String & feature_name,
    const String & transformation_model,
    const Param & transformation_model_params)
  {
    // extract out the calibration points
    TransformationModel::DataPoints data;
    TransformationModel::DataPoint point;
    for (size_t i = 0; i < component_concentrations.size(); i++){
      point.first = component_concentrations[i].actual_concentration;
      point.second = component_concentrations[i].feature.getMetaValue(feature_name);
      data.push_back(point);
    }

    // fit the data to the model
    AbsoluteQuantitationMethod aqm;
    Param params = aqm.fitTransformationModel(transformation_model, data, transformation_model_params);

    // store the information about the fit
    return params;
  }
  
  double AbsoluteQuantitation::applyCalibration(Feature & component,
    Feature & IS_component,
    String & feature_name,
    String & transformation_model,
    Param & transformation_model_params)
  {
    // calculate the ratio
    double ratio = calculateRatio(component, IS_component, feature_name);

    // calculate the absolute concentration
    double calculated_concentration = 0.0;
    AbsoluteQuantitationMethod aqm;
    calculated_concentration = aqm.evaluateTransformationModel(
      transformation_model, ratio, transformation_model_params);

    // check for less than zero
    if (calculated_concentration < 0.0)
    {
      calculated_concentration = 0.0;
    }

    return calculated_concentration;
  }

  void AbsoluteQuantitation::quantifyComponents(FeatureMap& unknowns)
  {
    //Potential Optimizations: create a map for each unknown FeatureMap
    // to reduce multiple loops

    // initialize all quant_method variables
    std::map<String,AbsoluteQuantitationMethod>::iterator quant_methods_it;
    String quant_component_name; //i.e., quant_method transition_id
    String quant_IS_component_name; //i.e., quant_method internal standard transition_id
    String quant_feature_name; //i.e., quant_method peak_apex_int or peak_area
    String transformation_model;
    Param transformation_model_params;

    // initialize all unknown variables
    String component_name; //i.e., transition_id
    String IS_component_name; //i.e., internal standard transition_id
    String component_group_name; //i.e., peptideRef
    double calculated_concentration;
    String concentration_units;

    // initalize all other variables
    bool IS_found;
    Feature empty_feature;
    size_t IS_component_it, IS_component_group_it;

    // // iterate through the unknowns
    // for (size_t i = 0; i < unknowns.size(); i++)
    // {      

    // iterate through each component_group/feature     
    for (size_t feature_it = 0; feature_it < unknowns.size(); ++feature_it)
    {
      component_group_name = (String)unknowns[feature_it].getMetaValue("PeptideRef");
      Feature unknowns_quant_feature;

      // iterate through each component/sub-feature
      for (size_t sub_it = 0; sub_it < unknowns[feature_it].getSubordinates().size(); ++sub_it)
      {
        component_name = (String)unknowns[feature_it].getSubordinates()[sub_it].getMetaValue("native_id");  

        // apply the calibration curve to components that are in the quant_method
        if (quant_methods_.count(component_name)>0)
        { 
          calculated_concentration = 0.0;    
          quant_methods_it = quant_methods_.find(component_name);
          quant_methods_it->second.getComponentISFeatureNames(quant_component_name,quant_IS_component_name,quant_feature_name);
          if (quant_IS_component_name != "")
          {
            // look up the internal standard for the component
            IS_found = false;
            // Optimization: 90% of the IS will be in the same component_group/feature
            for (size_t is_sub_it = 0; is_sub_it < unknowns[feature_it].getSubordinates().size(); ++is_sub_it)
            {
              IS_component_name = (String)unknowns[feature_it].getSubordinates()[is_sub_it].getMetaValue("native_id");              
              if (quant_IS_component_name == IS_component_name)
              {
                IS_found = true;
                IS_component_group_it = feature_it;
                IS_component_it = is_sub_it;
                break;
              }
            }
            if (!IS_found)
            {// expand IS search to all components                
              // iterate through each component_group/feature     
              for (size_t is_feature_it = 0; is_feature_it < unknowns.size(); ++is_feature_it)
              {
                //iterate through each component/sub-feature
                for (size_t is_sub_it = 0; is_sub_it < unknowns[is_feature_it].getSubordinates().size(); ++is_sub_it)
                {
                  IS_component_name = (String)unknowns[is_feature_it].getSubordinates()[is_sub_it].getMetaValue("native_id");                   
                  if (quant_IS_component_name == IS_component_name)
                  {
                    IS_found = true;
                    IS_component_group_it = is_feature_it;
                    IS_component_it = is_sub_it;
                    break;
                  }
                }
                if (IS_found)
                {
                  break;
                }
              }
            }
            if (IS_found)
            {                
              quant_methods_it->second.getTransformationModel(transformation_model,transformation_model_params);
              calculated_concentration = applyCalibration(
                unknowns[feature_it].getSubordinates()[sub_it],
                unknowns[IS_component_group_it].getSubordinates()[IS_component_it],
                quant_feature_name,transformation_model,transformation_model_params);
            }
            else 
            {                
              LOG_INFO << "Component " << component_name << " IS " << quant_IS_component_name << " was not found.";
              LOG_INFO << "No concentration will be calculated.";
            }
          }
          else
          {
            quant_methods_it->second.getTransformationModel(transformation_model,transformation_model_params);
            calculated_concentration = applyCalibration(
              unknowns[feature_it].getSubordinates()[sub_it],
              empty_feature,
              quant_feature_name,transformation_model,transformation_model_params);
          }

          // add new metadata (calculated_concentration, concentration_units) to the component
          unknowns[feature_it].getSubordinates()[sub_it].setMetaValue("calculated_concentration",calculated_concentration);
          quant_methods_it->second.getConcentrationUnits(concentration_units);
          unknowns[feature_it].getSubordinates()[sub_it].setMetaValue("concentration_units",concentration_units);
          // calculate the bias?
        }
        else 
        {
          LOG_INFO << "Component " << component_name << " does not have a quantitation method.";
          LOG_INFO << "No concentration will be calculated.";
          unknowns[feature_it].getSubordinates()[sub_it].setMetaValue("calculated_concentration","");
          unknowns[feature_it].getSubordinates()[sub_it].setMetaValue("concentration_units","");
        }
      }
    }
    // }
  }

  void AbsoluteQuantitation::optimizeCalibrationCurveBruteForce(
    std::vector<AbsoluteQuantitationStandards::featureConcentration> & component_concentrations,
    String & feature_name,
    String & transformation_model,
    Param & transformation_model_params,
    Param & optimized_params)
  {
    
    double biases;
    double r2;
    int n_points;
    bool bias_check;

    //TODO use internal params
    int min_points = 4;
    double max_bias = 30.0;
    double min_r2 = 0.9; 
    int max_outliers = 1;  

    size_t n_loops;
    std::vector<AbsoluteQuantitationStandards::featureConcentration> component_concentrations_sub;
    std::vector<AbsoluteQuantitationStandards::featureConcentration>::const_iterator component_start_it;
    std::vector<AbsoluteQuantitationStandards::featureConcentration>::const_iterator component_end_it;

    // sort from min to max concentration

    // loop from all points to min_points
    for (size_t n_points = component_concentrations.size(); n_points >= min_points; --n_points)
    {
      n_loops = component_concentrations.size() - n_points;
      for (size_t  component_it = 0; component_it < n_loops; ++component_it)
      {
        // extract out components
        component_start_it = component_concentrations.begin() + component_it;
        component_end_it = component_concentrations.begin() + component_it + n_points;
        component_concentrations_sub(component_start_it, component_end_it);

        // fit the model
        optimized_params = fitCalibration(component_concentrations_sub,
          feature_name,
          transformation_model,
          transformation_model_params)

        // calculate the R2 and bias
        calculateBiasAndR2(
          const std::vector<AbsoluteQuantitationStandards::featureConcentration> & component_concentrations,
          feature_name,
          transformation_model,
          transformation_model_params,
          std::vector<double> biases,
          double r2);
        
        // check R2 and biases
        bias_check = true;
        for (size_t bias_it = 0; bias_it != biases.size(); --bias_it){
          if (biases[bias_it] > max_bias)
          {
            bias_check = false;
          }
        }
        if (bias_check && r2 > min_r2)
        {
          break;
        }      

      }
    }
    LOG_INFO << "Valid calibration not found for " << component_concentrations[0].feature.getPeptideRef() << " .";
  }
  
  void AbsoluteQuantitation::calculateBiasAndR2(
    const std::vector<AbsoluteQuantitationStandards::featureConcentration> & component_concentrations,
    const String & feature_name,
    const String & transformation_model,
    const Param & transformation_model_params,
    std::vector<double> biases,
    double r2)
  {
    
    // extract out the calibration points
    const std::vector<double> concentration_ratios, feature_amounts_ratios;
    for (size_t i = 0; i < component_concentrations.size(); i++){
      concentrations.push_back(component_concentrations[i].actual_concentration);
      feature_amount.push_back(component_concentrations[i].feature.getMetaValue(feature_name));

      // calculate the actual and calculated concentration ratios
      const double calculated_concentration_ratio = applyCalibration(component_concentrations[i].feature,
        component_concentrations[i].IS_feature,
        feature_name,
        transformation_model,
        transformation_model_params);

      const double actual_concentration_ratio = component_concentrations[i].actual_concentration/component_concentrations[i].IS_actual_concentration;
      concentration_ratios.push_back(component_concentrations[i].actual_concentration);

      // calculate the bias
      const double bias = calculateBias(actual_concentration_ratio, calculated_concentration_ratio);
      biases.push_back(bias);

      // extract out the feature amount ratios
      const double feature_amount_ratio = calculateRatio(component_concentrations[i].feature,
        component_concentrations[i].IS_feature,
        feature_name);
      feature_amounts_ratios.push_back(feature_amount_ratio);
    }

    // calculate the R2 
    const double r_value = Statistics::pearsonCorrelationCoefficient(
      concentration_ratios.begin(), concentration_ratios.begin() + concentration_ratios.size(),
      feature_amounts_ratios.begin(), feature_amounts_ratios.begin() + feature_amounts_ratios.size()
    );
    r2 = r_value*r_value;

  }

} // namespace

