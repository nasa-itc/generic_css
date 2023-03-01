/* Copyright (C) 2016 - 2021 National Aeronautics and Space Administration. All Foreign Rights are Reserved to the U.S. Government.

   This software is provided "as is" without any warranty of any, kind either express, implied, or statutory, including, but not
   limited to, any warranty that the software will conform to, specifications any implied warranties of merchantability, fitness
   for a particular purpose, and freedom from infringement, and any warranty that the documentation will conform to the program, or
   any warranty that the software will be error free.

   In no event shall NASA be liable for any damages, including, but not limited to direct, indirect, special or consequential damages,
   arising out of, resulting from, or in any way connected with the software or its documentation.  Whether or not based upon warranty,
   contract, tort or otherwise, and whether or not loss was sustained from, or arose out of the results of, or use of, the software,
   documentation or services provided hereunder

   ITC Team
   NASA IV&V
   ivv-itc@lists.nasa.gov
*/

#include <generic_css_data_point.hpp>
#include <ItcLogger/Logger.hpp>

namespace Nos3
{
    extern ItcLogger::Logger *sim_logger;

    Generic_cssDataPoint::Generic_cssDataPoint(double count)
    {
        sim_logger->trace("Generic_cssDataPoint::Generic_cssDataPoint:  Defined Constructor executed");

        /* Do calculations based on provided data */
        std::vector<bool>   valid(6, true);
        std::vector<float> illum(6, 0.0);
        _generic_css_data = illum;
        _generic_css_data[0] = count * 0.001;
        _generic_css_data[1] = count * 0.001;
        _generic_css_data[2] = count * 0.001;
        _generic_css_data[3] = count * 0.001;
        _generic_css_data[4] = count * 0.001;
        _generic_css_data[5] = count * 0.001;
    }

    Generic_cssDataPoint::Generic_cssDataPoint(double scaleFactor, int16_t spacecraft, const boost::shared_ptr<Sim42DataPoint> dp)
    {
        sim_logger->trace("Generic_cssDataPoint::Generic_cssDataPoint:  42 Constructor executed");

        /* Initialize data */
        std::vector<bool>   valid(6, false);
        std::vector<float> illum(6, 0.0);
        _generic_css_data = illum;

        /*
        ** Declare 42 telemetry string prefix
        ** 42 variables defined in `42/Include/42types.h`
        ** 42 data stream defined in `42/Source/IPC/SimWriteToSocket.c`
        */
        std::ostringstream MatchString;
        MatchString << "SC[" << spacecraft << "].AC.CSS";
        size_t MSsize = MatchString.str().size();

        /* Parse 42 telemetry */
        std::vector<std::string> lines = dp->get_lines();
        try 
        {
            for (int i = 0; i < lines.size(); i++) 
            {
                /* Debugging print
                sim_logger->debug("Line[%d] = %s", i, lines[i].c_str());
                */ 

                // Compare prefix
                if (lines[i].compare(0, MSsize, MatchString.str()) == 0) 
                {
                    size_t lb = lines[i].find_first_of("[", MSsize);
                    size_t rb = lines[i].find_first_of("]", MSsize);
                    int index = std::stoi(lines[i].substr(lb+1, rb-lb-1));
                    if ((index >= 0) && (index < numChannels)) {
                        std::string param(lines[i].substr(rb+2, 5));
                        size_t equal = lines[i].find_first_of("=");
                        std::string value(lines[i].substr(equal+1, lines[i].size()-equal-1));
                        if (param.compare("Valid") == 0) 
                        {
                            int flag = std::stoi(value);
                            if (flag != 0) 
                            {
                                valid[index] = true;
                            } else 
                            {
                                valid[index] = false;
                                _generic_css_data[index] = 0.0;
                            }
                        } else if (param.compare("Illum") == 0) 
                        {
                            _generic_css_data[index] = std::stof(value) / scaleFactor;
                            /* Debugging print
                            sim_logger->debug("  css[%d] stof(value) = %f ", index, std::stof(value));
                            sim_logger->debug("  css[%d] stof(value) / scaleFactor = %f ", index, std::stof(value) / scaleFactor);
                            */
                        }
                    }
                }
            }
        } 
        catch(const std::exception& e) 
        {
            /* Force data to be set to known values */
            std::vector<float> illum(6, 8.0);
            _generic_css_data = illum; 
            sim_logger->error("Generic_cssDataPoint::Generic_cssDataPoint:  Parsing exception %s", e.what());
        }
    }

    /* Used for printing a representation of the data point */
    std::string Generic_cssDataPoint::to_string(void) const
    {
        sim_logger->trace("Generic_cssDataPoint::to_string:  Executed");
        std::stringstream output;
        output << "Channel values: ";
        int i;
        for (i = 0; i < _generic_css_data.size(); i++) {
            output << _generic_css_data[i];
            if (i < _generic_css_data.size() - 1) {
                output << ", ";
            }
        }
        return output.str();

    }
} /* namespace Nos3 */
