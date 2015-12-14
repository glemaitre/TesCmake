/// \file nrc.h
/// \brief Numerical recipes header
///
/// \author C. Andre (c.andre@apeira-technologies.fr)
/// \version 1.0
/// \date September 2015
///
/// This code was developped by Apeira Technologies
/// Contact: m.voiry@apeira-technologies.fr

#ifndef ALGO_NRC_H
#define ALGO_NRC_H

#ifdef _MSC_VER
#pragma warning (disable : 4244 4305)
#endif

#include "nrcutil.h"
/// QR
#include "qrdcmp.h"
#include "qrsolv.h"
#include "qrsol.h"
/// Eigen system
#include "jacobi.h"
#include "householder.h"
#include "tqli.h"
#include "tred2.h"
#include "trevc.h"
#include "nsevd.h"
#include "hseqr.h"
#include "elmhes.h"
#include "balanc.h"
//#include "nrc/unpackq.h"

#ifdef _MSC_VER
#pragma warning (default : 4244 4305)
#endif
#endif
