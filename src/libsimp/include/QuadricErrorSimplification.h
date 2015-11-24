/*
 * QuadricErrorSimplification.h
 *
 *  Created on: Nov 24, 2015
 *      Author: kenkron
 */

#ifndef SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_
#define SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_

#include "Simplification.h"

class QuadricErrorSimplification: public Simplification {
public:
	QuadricErrorSimplification();
	virtual ~QuadricErrorSimplification();
};

#endif /* SRC_LIBSIMP_QUADRICERRORSIMPLIFICATION_H_ */
