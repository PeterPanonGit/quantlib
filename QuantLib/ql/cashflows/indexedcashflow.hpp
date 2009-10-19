/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2009 Chris Kenyon

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
 */

/*! \file simplecashflow.hpp
 \brief Cash flow dependent on an index ratio (NOT a coupon, i.e. no accruals)
 */

#ifndef quantlib_indexed_cash_flow_hpp
#define quantlib_indexed_cash_flow_hpp

#include <ql/cashflow.hpp>

namespace QuantLib {

    class Index;

    //! Cash flow dependent on an index ratio (NOT a coupon, i.e. no accruals)
    /*! We expect this to be used inside an istrument that does all the date
        adjustment etc., so this takes just dates and does not change them.
     */
    class IndexedCashFlow : public CashFlow {
      public:
        IndexedCashFlow(Real notional,
                        const boost::shared_ptr<Index> &index,
                        const Date& baseDate,
                        const Date& fixingDate,
                        const Date& paymentDate)
        : notional_(notional), index_(index),
          baseDate_(baseDate), fixingDate_(fixingDate),
          paymentDate_(paymentDate) {}
        //! \name Event interface
        //@{
        Date date() const { return paymentDate_; }
        //@}
        virtual Real notional() const { return notional_; }
        virtual Date baseDate() const { return baseDate_; }
        virtual Date fixingDate() const { return fixingDate_; }
        virtual boost::shared_ptr<Index> index() const { return index_; }
        //! \name CashFlow interface
        //@{
        Real amount() const;    // already virtual
        //@}
        //! \name Visitability
        //@{
        virtual void accept(AcyclicVisitor&);
        //@}
    private:
        Real notional_;
        boost::shared_ptr<Index> index_;
        Date baseDate_, fixingDate_, paymentDate_;
    };


    // inline definitions

    inline void IndexedCashFlow::accept(AcyclicVisitor& v) {
        Visitor<IndexedCashFlow>* v1 =
        dynamic_cast<Visitor<IndexedCashFlow>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            CashFlow::accept(v);
    }


}







#endif
