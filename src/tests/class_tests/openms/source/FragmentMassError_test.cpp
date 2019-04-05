// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2018.
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
// $Maintainer: Chris Bielow $
// $Authors: Swenja Wagner, Patricia Scheil $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////

#include <OpenMS/QC/FragmentMassError.h>

#include <vector>
#include <OpenMS/METADATA/PeptideHit.h>
#include <OpenMS/KERNEL/BaseFeature.h>
#include <OpenMS/METADATA/MetaInfoInterface.h>
#include <OpenMS/KERNEL/MSExperiment.h>

//////////////////////////

using namespace OpenMS;

START_TEST(FragmentMassError, "$Id$")

    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////

    //--------------------------------------------------------------------
    // Input Data
    //--------------------------------------------------------------------

    //-------------------------------------------------
    //create MSExperiment
    //-------------------------------------------------

    //create Peaks
    Peak1D himalaya_1(116, 42);
    Peak1D himalaya_2(566.4, 42);
    Peak1D himalaya_3(777, 42);
    Peak1D himalaya_4(805.422, 42);

    Peak1D alabama_1(45, 42);
    Peak1D alabama_2(112.5, 42);
    Peak1D alabama_3(146.8, 42);

    //MS_Empty
    MSSpectrum ms_spec_empty;

    //MS1Spectrum
    MSSpectrum ms_spec_1;
    ms_spec_1.setRT(5);
    ms_spec_1.setMSLevel(1);

    //MS_Himalaya
    MSSpectrum ms_spec_2_himalaya;
    ms_spec_2_himalaya.setRT(3.7);
    ms_spec_2_himalaya.setMSLevel(2);
    ms_spec_2_himalaya.push_back(himalaya_1);
    ms_spec_2_himalaya.push_back(himalaya_2);
    ms_spec_2_himalaya.push_back(himalaya_3);
    ms_spec_2_himalaya.push_back(himalaya_4);

    //MS_Alabama
    MSSpectrum ms_spec_2_alabama;
    ms_spec_2_alabama.setRT(2);
    ms_spec_2_alabama.setMSLevel(2);
    ms_spec_2_alabama.push_back(alabama_1);
    ms_spec_2_alabama.push_back(alabama_2);
    ms_spec_2_alabama.push_back(alabama_3);

    //MS_Exception
    MSSpectrum ms_spec_2_excp;
    ms_spec_2_excp.setRT(4);
    ms_spec_2_excp.setMSLevel(2);

    MSExperiment exp;
    exp.setSpectra({ ms_spec_1, ms_spec_2_himalaya, ms_spec_2_alabama});

    //-------------------------------------------------
    //create FeatureMap
    //-------------------------------------------------

    //PepHit Himalaya
    PeptideHit pep_hit_hi;
    pep_hit_hi.setSequence(AASequence::fromString("HIMALAYA"));

    //PepHit Alabama
    PeptideHit pep_hit_al;
    pep_hit_al.setSequence(AASequence::fromString("ALABAMA"));

    //PepID empty
    PeptideIdentification pep_id_empty;
    pep_id_empty.setRT(4);

    //PepID himalaya
    PeptideIdentification pep_id_hi;
    pep_id_hi.setRT(3.72);
    pep_id_hi.setMZ(888);
    pep_id_hi.setHits({pep_hit_hi});

    //PepID alabama
    PeptideIdentification pep_id_al;
    pep_id_al.setRT(2);
    pep_id_al.setMZ(264);
    pep_id_al.setHits({pep_hit_al});

    //PepID out of tolerance
    PeptideIdentification pep_id_tol_out;
    pep_id_tol_out.setRT(2.1);

    //PepID matches with ms1 spectrum
    PeptideIdentification pep_id_ms1;
    pep_id_ms1.setRT(5);

    //PepID peak_RT does not exist in msEp
    PeptideIdentification pep_id_notExist;
    pep_id_notExist.setRT(7);

    //Feature valid data
    Feature feat_valid;
    feat_valid.setPeptideIdentifications({pep_id_hi, pep_id_al});

    //Feature empty
    Feature feat_empty;

    //FeatureMap valid data
    FeatureMap fmap;
    //fmap.push_back(feat_empty);
    fmap.push_back(feat_valid);

    //FeatureMap tol_out
    FeatureMap fmap_tol_out;
    fmap_tol_out.setUnassignedPeptideIdentifications({pep_id_tol_out});

    //FeatureMap ms1
    FeatureMap fmap_ms1;
    fmap_ms1.setUnassignedPeptideIdentifications({pep_id_ms1});

    //FeatureMap tol_out
    FeatureMap fmap_notExist;
    fmap_notExist.setUnassignedPeptideIdentifications({pep_id_notExist});


    //--------------------------------------------------------------------
    // Tests
    //--------------------------------------------------------------------

    //////////////////////////////////////////////////////////////////
    //start Section
    /////////////////////////////////////////////////////////////////

    FragmentMassError* ptr = nullptr;
    FragmentMassError* nulpt = nullptr;
    START_SECTION(FragmentMassError())
        {
          ptr = new FragmentMassError();
          TEST_NOT_EQUAL(ptr, nulpt)
        }
    END_SECTION

    START_SECTION(~FragmentMassError())
        {
          delete ptr;
        }
    END_SECTION


    FragmentMassError frag_ma_err;
    //tests compute function
    START_SECTION(void compute(FeatureMap& fmap, const MSExperiment& exp, const double mz_tolerance = 20))
        {
          //test with valid input
          frag_ma_err.compute(fmap, exp);
          std::vector<FragmentMassError::FMEStatistics> result;
          result = frag_ma_err.getResults();

          TEST_REAL_SIMILAR(result[0].average_ppm, (0.034 + 0.088 + 1.38 - 5.01 + 0.531 - 1.449 - 0.23)/7.)
          TEST_REAL_SIMILAR(result[0].variance_ppm, 4.391826)
          /*
          ms2ir.compute(fmap, ms_exp);
          std::vector<Ms2IdentificationRate::IdentificationRateData> result;
          result = ms2ir.getResults();

          for (auto idrd : result)
          {
            TEST_EQUAL(idrd.num_peptide_identification, 2)
            TEST_EQUAL(idrd.num_ms2_spectra, 6)
            TEST_REAL_SIMILAR(idrd.identification_rate, 1./3)
          }
          */

          //empty ms experiment
          //TEST_EXCEPTION_WITH_MESSAGE(Exception::MissingInformation, ms2ir_empty_msexp.compute(fmap, ms_empty_exp), "MSExperiment is empty")

          //empty feature map
          /*
          ms2ir_empty_fmap.compute(fmap_empty, ms_exp);
          std::vector<Ms2IdentificationRate::IdentificationRateData> result_empty_fmap;
          result_empty_fmap = ms2ir_empty_fmap.getResults();

          for (auto idrd_empty_fmap : result_empty_fmap)
          {
            TEST_EQUAL(idrd_empty_fmap.num_peptide_identification, 0)
            TEST_EQUAL(idrd_empty_fmap.num_ms2_spectra, 6)
            TEST_REAL_SIMILAR(idrd_empty_fmap.identification_rate, 0)
          }
          */
        }
    END_SECTION


    START_SECTION(QCBase::Status requires() const override)
        {
          QCBase::Status stat = QCBase::Status() | QCBase::Requires::RAWMZML | QCBase::Requires::POSTFDRFEAT;
          TEST_EQUAL(stat, frag_ma_err.requires())
        }
    END_SECTION


    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
END_TEST