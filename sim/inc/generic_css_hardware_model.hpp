#ifndef NOS3_GENERIC_CSSHARDWAREMODEL_HPP
#define NOS3_GENERIC_CSSHARDWAREMODEL_HPP

/*
** Includes
*/
#include <map>

#include <boost/tuple/tuple.hpp>
#include <boost/property_tree/ptree.hpp>

#include <Client/Bus.hpp>
#include <I2C/Client/I2CSlave.hpp> /* TODO: Change if your protocol bus is different (e.g. SPI, I2C, etc.) */

#include <sim_i_data_provider.hpp>
#include <generic_css_data_point.hpp>
#include <sim_i_hardware_model.hpp>


/*
** Defines
*/
#define GENERIC_CSS_SIM_SUCCESS 0
#define GENERIC_CSS_SIM_ERROR   1


/*
** Namespace
*/
namespace Nos3
{
    /* Standard for a hardware model */
    class Generic_cssHardwareModel : public SimIHardwareModel
    {
    public:
        /* Constructor and destructor */
        Generic_cssHardwareModel(const boost::property_tree::ptree& config);
        ~Generic_cssHardwareModel(void);

    private:
        /* Private helper methods */
        void create_generic_css_hk(std::vector<uint8_t>& out_data); 
        void create_generic_css_data(std::vector<uint8_t>& out_data); 
        std::uint8_t determine_i2c_response_for_request(const std::vector<uint8_t>& in_data);
        void command_callback(NosEngine::Common::Message msg); /* Handle backdoor commands and time tick to the simulator */

        /* Private data members */
        class I2CSlaveConnection*                           _i2c_slave_connection; /* TODO: Change if your protocol bus is different (e.g. SPI, I2C, etc.) */
        std::unique_ptr<NosEngine::Client::Bus>             _time_bus; /* Standard */

        SimIDataProvider*                                   _generic_css_dp; /* Only needed if the sim has a data provider */

        /* Internal state data */
        std::uint8_t                                        _enabled;
        std::uint32_t                                       _count;
        std::uint32_t                                       _config;
        std::uint32_t                                       _status;
    };

    class I2CSlaveConnection : public NosEngine::I2C::I2CSlave
    {
    public:
        I2CSlaveConnection(Generic_cssHardwareModel* hm, int bus_address, std::string connection_string, std::string bus_name);
        size_t i2c_read(uint8_t *rbuf, size_t rlen);
        size_t i2c_write(const uint8_t *wbuf, size_t wlen);
    private:
        Generic_cssHardwareModel* _hardware_model;
        std::uint8_t _i2c_out_data;
    };
}

#endif
