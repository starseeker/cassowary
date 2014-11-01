// $Id: ClSolver.cc,v 1.4 2005/10/20 04:25:22 gjb Exp $

#ifdef HAVE_CONFIG_H
#include "config.h"
#define CONFIG_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClSolver.h"
#include "ClConstraint.h"
#include "ClErrors.h"
#include "ClTypedefs.h"

ClSolver &ClSolver::AddConstraint(ClConstraint *const) { return *this; }

std::ostream &PrintTo(std::ostream &xo, const ClConstraintSet &setCn) {
    ClConstraintSet::const_iterator it = setCn.begin();
    for (; it != setCn.end(); ++it) {
        const ClConstraint *pcn = *it;
        xo << *pcn << std::endl;
    }
    return xo;
}

std::ostream &PrintTo(std::ostream &xo, const std::list<FDNumber> &listFDN) {
    std::list<FDNumber>::const_iterator it = listFDN.begin();
    for (; it != listFDN.end();) {
        FDNumber n = *it;
        xo << n;
        ++it;
        if (it != listFDN.end())
            xo << ",";
    }
    return xo;
}

std::ostream &operator<<(std::ostream &xo, const ClConstraintSet &setCn) {
    return PrintTo(xo, setCn);
}

std::ostream &operator<<(std::ostream &xo, const ClSolver &solver) {
    return solver.PrintOn(xo);
}

std::ostream &operator<<(std::ostream &xo, const std::list<FDNumber> &listFDN) {
    return PrintTo(xo, listFDN);
}
