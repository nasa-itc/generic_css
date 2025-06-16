// ======================================================================
// \title  Generic_css.hpp
// \author jstar
// \brief  hpp file for Generic_css component implementation class
// ======================================================================

#ifndef Components_Generic_css_HPP
#define Components_Generic_css_HPP

#include "css_src/Generic_cssComponentAc.hpp"

namespace Components {

  class Generic_css :
    public Generic_cssComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Generic_css object
      Generic_css(
          const char* const compName //!< The component name
      );

      //! Destroy Generic_css object
      ~Generic_css();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command TODO
      //!
      //! TODO
      // void TODO_cmdHandler(
      //     FwOpcodeType opCode, //!< The opcode
      //     U32 cmdSeq //!< The command sequence number
      // ) override;
      void REQUEST_DATA_cmdHandler(
        FwOpcodeType opCode, 
        U32 cmdSeq
      ) override;

      void updateData_handler(
        const NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

  };

}

#endif
