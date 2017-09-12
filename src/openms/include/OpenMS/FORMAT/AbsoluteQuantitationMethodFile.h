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

#ifndef OPENMS_FORMAT_ABSOLUTEQUANTITATIONMETHODFILE_H
#define OPENMS_FORMAT_ABSOLUTEQUANTITATIONMETHODFILE_H

#include <OpenMS/FORMAT/CsvFile.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/DATASTRUCTURES/StringListUtils.h>
#include <OpenMS/ANALYSIS/QUANTITATION/AbsoluteQuantitationMethod.h>

namespace OpenMS
{
  /**
      @brief File adapter for AbsoluteQuantitationMethod files.

      loads and stores .csv or .tsv files describing an AbsoluteQuantitationMethod.

      @ingroup FileIO
  */
  class OPENMS_DLLAPI AbsoluteQuantitationMethodFile :
    private CsvFile,
    public ProgressLogger
  {
public:
    ///Default constructor
    AbsoluteQuantitationMethodFile();
    ///Destructor
    virtual ~AbsoluteQuantitationMethodFile();

    /**
        @brief Loads an AbsoluteQuantitationMethod file.

        @exception Exception::FileNotFound is thrown if the file could not be opened
        @exception Exception::ParseError is thrown if an error occurs during parsing
    */
    void load(const String & filename, AbsoluteQuantitationMethod & aqm);

    /**
        @brief Stores an AbsoluteQuantitationMethod file.

        @exception Exception::UnableToCreateFile is thrown if the file could not be created
    */
    void store(const String & filename, const AbsoluteQuantitationMethod & aqm) const;
    
    /**
        @brief Checks if a file is valid with respect to the mapping file and the controlled vocabulary.

        @param line Header line of the .csv file.
        @param headers A map of header strings to column positions.
        @param params_headers A map of transformation model parameter header strings to column positions.
    */
    void parseHeader(StringList & line, std::map<std::string,int> & headers,
        std::map<std::string,std::vector<int>> & params_headers)

    /**
        @brief parses a line into the members of AbsoluteQuantitationMethod.

        @param line line of the .csv file.
        @param aqm AbsoluteQuantitationMethod.
    */
    void parseLine(StringList & line, AbsoluteQuantitationMethod & aqm);
    
    /**
        @brief Checks if a file is valid with respect to the mapping file and the controlled vocabulary.

        @param line line of the .csv file.
        @param aqm AbsoluteQuantitationMethod.
    */
    void parseTransformationModelParameters(StringList & line, AbsoluteQuantitationMethod & aqm);

private:

  };

} // namespace OpenMS

#endif // OPENMS_FORMAT_ABSOLUTEQUANTITATIONMETHODFILE_H