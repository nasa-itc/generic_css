#ifndef NOS3_GENERIC_CSSDATAPOINT_HPP
#define NOS3_GENERIC_CSSDATAPOINT_HPP

#include <boost/shared_ptr.hpp>
#include <sim_42data_point.hpp>

namespace Nos3
{
    /* Standard for a data point used transfer data between a data provider and a hardware model */
    class Generic_cssDataPoint : public SimIDataPoint
    {
    public:
        /* Constructors */
        Generic_cssDataPoint(double count);
        Generic_cssDataPoint(int16_t spacecraft, const boost::shared_ptr<Sim42DataPoint> dp);

        /* Accessors */
        /* Provide the hardware model a way to get the specific data out of the data point */
        std::string to_string(void) const;
        double      get_generic_css_data_voltage_1(void) const {return _generic_css_data[0];}
        double      get_generic_css_data_voltage_2(void) const {return _generic_css_data[1];}
        double      get_generic_css_data_voltage_3(void) const {return _generic_css_data[2];}
        double      get_generic_css_data_voltage_4(void) const {return _generic_css_data[3];}
        double      get_generic_css_data_voltage_5(void) const {return _generic_css_data[4];}
        double      get_generic_css_data_voltage_6(void) const {return _generic_css_data[5];}
        bool        is_generic_css_data_valid(void) const {return _generic_css_data_is_valid;}
    
    private:
        /* Disallow these */
        Generic_cssDataPoint(void) {};
        Generic_cssDataPoint(const Generic_cssDataPoint&) {};
        ~Generic_cssDataPoint(void) {};

        /* Specific data you need to get from the data provider to the hardware model */
        /* You only get to this data through the accessors above */
        mutable bool   _generic_css_data_is_valid;
        mutable double _generic_css_data[6];
    };
}

#endif
