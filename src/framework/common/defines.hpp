//----------------------------------------------------------------------------------------
/**
* \file       defines.hpp
* \author     Yury Udavichenka
* \date       20/02/2022
* \brief      Defines
*
*  Header containing various defines used by the framework
*  Initially extracted out of [logging.hpp] because some things
*  did not really belong there semantically
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#ifdef _WIN32
#define SE_SPRINTF sprintf_s
#else
#define SE_SPRINTF sprintf
#endif

#define SE_UNUSED(x) (void)(x)