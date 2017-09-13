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
//

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////

#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitationMethodFile.h>

using namespace OpenMS;
using namespace std;

///////////////////////////

START_TEST(AbsoluteQuantitationMethodFile, "$Id$")

/////////////////////////////////////////////////////////////

AbsoluteQuantitationMethodFile* ptr = 0;
AbsoluteQuantitationMethodFile* nullPointer = 0;

START_SECTION((AbsoluteQuantitationMethodFile()))
	ptr = new AbsoluteQuantitationMethodFile();
	TEST_NOT_EQUAL(ptr, nullPointer)
END_SECTION

START_SECTION((~AbsoluteQuantitationMethodFile()))
	delete ptr;
END_SECTION

START_SECTION((void parseHeader(StringList & line, std::map<std::string,int> & headers,
  std::map<std::string,int> & params_headers)))
  
  AbsoluteQuantitationMethodFile aqmf;
  
  std::map<std::string,int> headers;
  std::map<std::string,int> params_headers;

  // header test 1
  StringList header1; 
  header1.push_back("IS_name");
  header1.push_back("component_name");
  header1.push_back("feature_name");
  header1.push_back("concentration_units");
  header1.push_back("llod");
  header1.push_back("ulod");
  header1.push_back("lloq");
  header1.push_back("uloq");
  header1.push_back("correlation_coefficient");
  header1.push_back("actual_concentration");
  header1.push_back("n_points");
  header1.push_back("transformation_model");
  header1.push_back("transformation_model_param_slope)";
  header1.push_back("transformation_model_param_intercept)";

  aqmf.parseHeader(header1, headers, params_headers);

  TEST_EQUAL(headers["IS_name"],0);
  TEST_EQUAL(headers["transformation_model"],11);
  TEST_EQUAL(params_headers["slope"],12);
  TEST_EQUAL(params_headers["intercept"],13);
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
