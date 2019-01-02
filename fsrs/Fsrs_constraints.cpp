#include "Fsrs_instance.hpp"
#include <algebraLib/BitExtract.hpp>
#include "languages/Bair/BairInstance.hpp"
#include "languages/Bair/BairWitness.hpp"
#include "Fsrs.hpp"
#define ttgenRand (Algebra::one()) 

using Algebra::degreeOfProduct;
using Algebra::FieldElement;
using Algebra::generateRandom;
using Algebra::one;
using Algebra::PolynomialDegree;
using Algebra::PolynomialInterface;


namespace simple_fsrs {
namespace ACSP_FOR_FSRS{

namespace { // anonymous namespace for polys and common vars
    using Algebra::mapIntegerToFieldElement;

class polyAdd_class : public PolynomialInterface {
    public:
        polyAdd_class() {};

        Algebra::FieldElement eval(const std::vector<FieldElement>& x)const{
            return Fsrs::evalp::ep(x);
        }

        bool isEffectiveInput(const size_t varId)const{
            switch(varId)
            {                                
                default: return true;
            }
        }

        Algebra::PolynomialDegree getDegreeBound(const std::vector<PolynomialDegree>& inputDegrees)const{

            struct ttdeg{
                long long deg_;
                ttdeg(const PolynomialDegree& d):deg_(PolynomialDegree::integral_t(d)){};
                ttdeg(const long long& d):deg_(d){};

                ttdeg operator*(const ttdeg& d)const{
                    return degreeOfProduct(PolynomialDegree(deg_),PolynomialDegree(d.deg_));
                }
                
                ttdeg operator+(const ttdeg& d)const{
                    return std::max(deg_,d.deg_);
                }
            };

            const ttdeg A = ttdeg(inputDegrees[Fsrs::reg::A]);          
            const ttdeg A_next = ttdeg(inputDegrees[Fsrs::reg::A + Fsrs::NUMREGS]);
            const ttdeg B = ttdeg(inputDegrees[Fsrs::reg::B]);
            const ttdeg B_next = ttdeg(inputDegrees[Fsrs::reg::B + Fsrs::NUMREGS]);
            const ttdeg C = ttdeg(inputDegrees[Fsrs::reg::C]);
            const ttdeg C_next = ttdeg(inputDegrees[Fsrs::reg::C + Fsrs::NUMREGS]);

            const ttdeg resTmp = (A + B + C) + (A_next + A) + (B_next + B) + (C_next + C);

            return PolynomialDegree(resTmp.deg_);
        }

        std::unique_ptr<PolynomialInterface> clone()const{
            return std::unique_ptr<PolynomialInterface>(new polyAdd_class);
        }

        size_t numVars()const{
            return Fsrs::NUMREGS;
        }
};
} 

    Fsrs_CS::Fsrs_CS() {
        polys_.push_back(polyPtr_t(new polyAdd_class()));
    }

    Fsrs_CS* Fsrs_CS::clone() const{
        return new Fsrs_CS();
    }

    using std::vector;
    vector<FieldElement> Fsrs_CS::eval(const vector<FieldElement>& assignment) const{
        vector<FieldElement> res;
        for(const auto& p: polys_){
            res.push_back(p->eval(assignment));
        }
        return res;
    }

} // ACSP_FOR_FSRS namespace
} // simple_fsrs namespace