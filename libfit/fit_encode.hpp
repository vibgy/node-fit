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


#if !defined(FIT_ENCODE_HPP)
#define FIT_ENCODE_HPP

#include <iosfwd>
#include <vector>
#include "fit.hpp"
#include "fit_mesg.hpp"
#include "fit_mesg_definition.hpp"
#include "fit_mesg_definition_listener.hpp"
#include "fit_mesg_listener.hpp"
#include "fit_protocol_validator.hpp"

namespace fit
{

class Encode
    : public MesgListener
    , public MesgDefinitionListener
{
public:
    DEPRECATED
        (
        "Encode now supports multiple protocol version encoding use: "
        "Encode::Encode(fit::ProtocolVersion) to ensure features "
        "are correctly validated"
        )
    Encode(void);
    Encode( ProtocolVersion version );

    void Open(std::iostream& file);
    void Write(const MesgDefinition& mesgDef);
    void Write(const Mesg& mesg);
    void Write(const std::vector<Mesg>& mesgs);
    FIT_BOOL Close(void);
    void OnMesg(Mesg& mesg);
    void OnMesgDefinition(MesgDefinition& mesgDef);

private:
    void WriteFileHeader();

    MesgDefinition lastMesgDefinition[FIT_MAX_LOCAL_MESGS];
    FIT_UINT32 dataSize;
    std::iostream *file;

    ProtocolVersion version;
    ProtocolValidator validator;
};

} // namespace fit

#endif // defined(FIT_ENCODE_HPP)
