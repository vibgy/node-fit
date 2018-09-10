////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Dynastream Innovations Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2017 Dynastream Innovations Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 20.42Release
// Tag = production/akw/20.42.00-0-gffb87a0
////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_EXD_SCREEN_CONFIGURATION_MESG_LISTENER_HPP)
#define FIT_EXD_SCREEN_CONFIGURATION_MESG_LISTENER_HPP

#include "fit_exd_screen_configuration_mesg.hpp"

namespace fit
{

class ExdScreenConfigurationMesgListener
{
public:
    virtual ~ExdScreenConfigurationMesgListener() {}
    virtual void OnMesg(ExdScreenConfigurationMesg& mesg) = 0;
};

} // namespace fit

#endif // !defined(FIT_EXD_SCREEN_CONFIGURATION_MESG_LISTENER_HPP)
