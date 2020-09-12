// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
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
// $Maintainer: Ahmed Khalil $
// $Authors: Ahmed Khalil $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/config.h>

//Kernal classes
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/KERNEL/Feature.h>

//Standard library
 #include <cstddef> // for size_t & ptrdiff_t
 #include <vector>
 #include <string>
 #include <cmath>
 #include <numeric>
 #include <algorithm>

namespace OpenMS
{

  /**
    @brief IsotopeLabelingMDVs is a class to support and analyze isotopic labeling experiments
            (i.e. MDVs : Mass Distribution Vectors, also known as Mass Isotopomer Distribution (MID))
    
  */
  class OPENMS_DLLAPI IsotopeLabelingMDVs 
  {

  public:
    //@{
    /// Constructor
    IsotopeLabelingMDVs();

    /// Destructor
    ~IsotopeLabelingMDVs();
    //@}
 
    /**
      @brief This function performs an isotopic correction to account for unlabeled abundances coming from
      the derivatization agent (e.g., tBDMS) using correction matrix method and is calculated as follows:
      MDV_corrected = CM^-1 * MDV_normalized_feature
      The formula is obtained from "The importance of accurately correcting for the natural abundance of stable isotopes",
      Midani et al, doi:10.1016/j.ab.2016.12.011
     
      @param[in]  normalized_feature Feature with normalized values for each component and unlabeled chemical formula for each component group.
      @param[in]  correction_matrix  Square matrix holding correction factors derived either experimentally or theoretically which describe how spectral peaks of
      naturally abundant 13C contribute to spectral peaks that overlap (or convolve) the spectral peaks of the corrected MDV of the derivatization agent.
      @param[out] corrected_feature Feature with corrected values for each component.
    */
    void isotopicCorrection(
      Feature& normalized_feature, Feature& corrected_feature,
      std::vector<std::vector<double>> correction_matrix);
    
    /**
      @brief This function performs an isotopic correction to account for unlabeled abundances coming from
      the derivatization agent (e.g., tBDMS) using correction matrix method and is calculated as follows:
      MDV_corrected = CM^-1 * MDV_normalized_feature
      The formula is obtained from "The importance of accurately correcting for the natural abundance of stable isotopes",
      Midani et al, doi:10.1016/j.ab.2016.12.011
     
      @param[in]  normalized_featuremap FeatureMap with normalized values for each component and unlabeled chemical formula for each component group.
      @param[in]  correction_matrix Square matrix holding correction factors derived either experimentally or theoretically which describe how spectral peaks of
      naturally abundant 13C contribute to spectral peaks that overlap (or convolve) the spectral peaks of the corrected MDV of the derivatization agent.
      @param[out] corrected_featuremap FeatureMap with corrected values for each component.
    */
    void isotopicCorrections(
      FeatureMap& normalized_featureMap, FeatureMap& corrected_featureMap,
      std::vector<std::vector<double>> correction_matrix);

    /**
      @brief This function calculates the isotopic purity of the MDV using the following formula:
      isotopic purity of tracer (atom % 13C) = n / [n + (M + n-1)/(M + n)],
      where n in M+n is represented as the index of the result.
      The formula is extracted from "High-resolution 13C metabolic flux analysis",
      Long et al, doi:10.1038/s41596-019-0204-0

      @param[in]  normalized_feature Feature with normalized values for each component and the number of heavy labeled e.g., carbons.
      @param[in]  experiment_data Experiment data in percent.
      @param[in]  isotopic_purity_name Name of the isotopic purity tracer to be saved as a meta value.
      @param[out] feature_with_isotopic_purity Feature with the calculated isotopic purity for the component group.
    */
    void calculateIsotopicPurity(
      Feature& normalized_feature, Feature& feature_with_isotopic_purity,
      std::vector<double>& experiment_data, std::string& isotopic_purity_name);
    
    /**
      @brief This function calculates the isotopic purity of the MDVs using the following formula:
      isotopic purity of tracer (atom % 13C) = n / [n + (M + n-1)/(M + n)],
      where n in M+n is represented as the index of the result.
      The formula is extracted from "High-resolution 13C metabolic flux analysis",
      Long et al, doi:10.1038/s41596-019-0204-0

      @param[in]  normalized_featureMap FeatureMap with normalized values for each component and the number of heavy labeled e.g., carbons.
      @param[in]  experiment_data Experiment data in percent.
      @param[in]  isotopic_purity_name Name of the isotopic purity tracer to be saved as a meta value.
      @param[out] featureMap_with_isotopic_purity FeatureMap with the calculated isotopic purity for the component group.
    */
    void calculateIsotopicPurities(
      FeatureMap& normalized_featureMap, FeatureMap& featureMap_with_isotopic_purity,
      std::vector<double>& experiment_data, std::string& isotopic_purity_name);

    /**
      @brief This function calculates the accuracy of the MDV as compared to the theoretical MDV (only for 12C quality control experiments)
      using average deviation to the mean.
   
      @param[in]  normalized_feature Feature with normalized values for each component and the chemical formula of the component group.
      @param[in]  fragment_isotopomer_measured Measured scan values.
      @param[in]  fragment_isotopomer_theoretical Theoretical scan values.
      @param[out] feature_with_accuracy_info Feature with the component group accuracy and accuracy for the error for each component.
    */
    void calculateMDVAccuracy(
      Feature& normalized_feature, Feature& feature_with_accuracy_info,
      std::vector<double>& fragment_isotopomer_measured, std::vector<double>& fragment_isotopomer_theoretical);
    
    /**
       @brief This function calculates the accuracy of the MDVs as compared to the theoretical MDVs (only for 12C quality control experiments)
       using average deviation to the mean.
    
       @param[in]  normalized_featuremap FeatureMap with normalized values for each component and the chemical formula of the component group.
       @param[in]  fragment_isotopomer_measured Measured scan values.
       @param[in]  fragment_isotopomer_theoretical Theoretical scan values.
       @param[out] featuremap_with_accuracy_info FeatureMap with the component group accuracy and accuracy for the error for each component.
    */
    void calculateMDVAccuracies(
      FeatureMap& normalized_featureMap, FeatureMap& featureMap_with_accuracy_info,
      std::vector<double>& fragment_isotopomer_measured, std::vector<double>& fragment_isotopomer_theoretical);
 
    /**
      @brief This function calculates the mass distribution vector (MDV)
      either normalized to the highest mass intensity (norm_max) or normalized
      to the sum of all mass intensities (norm_sum)
     
      @param[in]   mass_intensity_type Mass intensity type (either norm_max or norm_sum).
      @param[in]   feature_name Feature name to determine which features are needed to apply calculations on.
      @param[in]   measured_feature Feature with measured intensity for each component.
      @param[out]  normalized_feature Feature with normalized values for each component.
    */
    void calculateMDV(
      Feature& measured_feature, Feature& normalized_feature,
      const String& mass_intensity_type, const String& feature_name);
    
    /**
      @brief This function calculates the mass distribution vector (MDV)
      either normalized to the highest mass intensity (norm_max) or normalized
      to the sum of all mass intensities (norm_sum)
     
      @param[in]   mass_intensity_type Mass intensity type (either norm_max or norm_sum).
      @param[in]   feature_name Feature name to determine which features are needed to apply calculations on.
      @param[in]   measured_featuremap FeatureMap with measured intensity for each component.
      @param[out]  normalized_featuremap FeatureMap with normalized values for each component.
    */
    void calculateMDVs(
      FeatureMap& measured_featureMap, FeatureMap& normalized_featureMap,
      const String& mass_intensity_type, const String& feature_name);
    
    
  protected:
    /**
      @brief This function performs an efficient matrix inversion
    
      @param[in]   correction_matrix Correction matrix of size nxn
      @param[out]  correction_matrix_inversed Correction matrix inversed of size nxn
    */
    template<typename T>
    void inverseMatrix_(std::vector<std::vector<T>>& correction_matrix, std::vector<std::vector<T>>& correction_matrix_inversed);
     
  };

}
