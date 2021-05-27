#include <Eigen/Core>
#include <Eigen/SVD>
#include <algorithm>

#include "coprime_factorization.h"
#include "polynomial_matrix.h"

namespace poly
{
using Matrixd = Matrix<double>;

auto get_F(const PolynomialFractionMatrix &mf)
{
    auto [Nl, Dl] = get_left(mf);
    Nl.transposeInPlace();
    Dl.transposeInPlace();
    const auto Nl_T = to_coeff_matrix(Nl);
    const auto Dl_T = to_coeff_matrix(Dl);

    Matrixd ret{/*matrix*/ {Nl_T.rows() + Dl_T.rows(),
                            std::max(Nl_T.cols(), Dl_T.cols())},
                /*deg*/ std::max(Nl_T.deg, Dl_T.deg)};

    // ret = [ Dl_T;
    //         Nl_T ]
    ret.matrix.topLeftCorner(Dl_T.rows(), Dl_T.cols()) = Dl_T.matrix;
    ret.matrix.bottomLeftCorner(Nl_T.rows(), Nl_T.cols()) = Nl_T.matrix;

    return ret;
}

auto send_selected_rows(const Matrixd &S, const Eigen::VectorXi &sel)
{
    const auto &mat = S.matrix;
    decltype(S.matrix) ret(sel.sum(), mat.cols());
    // typename Matrix<T>::EigenMatrix ret(sel.sum(), mat.cols());
    int rownew = 0;
    for (int i = 0; i < mat.rows(); ++i) {
        if (sel[i]) {
            ret.row(rownew) = mat.row(i);
            rownew++;
        }
    }
    return ret;
}

void indep(const Eigen::MatrixXd &S)
{
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    // std::cout << S << std::endl;
    int ind = 1;
    int i = 2;
    while (ind && i <= S.rows()) {
        const auto S_ = S.topRows(i);
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(S_ * S_.transpose());
        const auto sigma = svd.singularValues();
        const auto len = sigma.size() - 1;
        // std::cout << sigma << std::endl;
        // std::cout << sigma.size() << std::endl;

        if (sigma(len) / sigma(0) <
            (eps * std::max(i, static_cast<int>(S.cols())))) {
            ind = 0;
        } else {
        }

        break; // FIXME
    }

    // gap = 1.0e8;
    // [rows,cols] = size(S);
    // ind = 1;
    // i = 2;
    // while ind & i <= rows
    //     shortS = S(1:i,:);
    //     sigma = svd(shortS*shortS');
    //     len = length(sigma);
    //     if(sigma(len)/sigma(1) < (eps*max(i,cols)))
    //         ind =0;
    //     else
    //         shsig = [sigma(2:len);sigma(len)];
    //         if any( (sigma ./shsig) > gap)
    //             ind = 0;
    //         else
    //             ind = 1;
    //             i = i+1;
    //         end
    //     end
    // end
    // if ind
    //     b =[];
    // else
    //     c = S(i,:)/S(1:i-1,:);
    //     c = makezero(c,gap);
    //     b = [-c 1];
    // end
}

void t1calc(const Matrixd &S, const int Srows, Matrixd &T1,
            Eigen::VectorXi &sel, const int Frows, const int Fbcols,
            const Eigen::VectorXi &abar)
{
    Eigen::VectorXi b = Eigen::VectorXi::Ones(1);
    while ((T1.rows() < Frows - Fbcols) && sel.any() && b.size() != 0) {
        indep(send_selected_rows(S, sel));
        break; // FIXME
    }
}

void coprime_factorization(const PolynomialFractionMatrix &mf)
{
    auto F = get_F(mf);
    auto S = F;

    Eigen::VectorXd pr;

    Eigen::VectorXi sel = Eigen::VectorXi::Ones(F.rows());
    auto abar = Eigen::VectorXi::LinSpaced(F.rows() - F.bcols(), F.bcols() + 1,
                                           F.rows());
    auto shft = 0;

    Matrixd T1{/*matrix*/ {},
               /*deg*/ 1};
    auto T1bcols = 1;

    while (T1.is_empty() || T1.rows() < F.rows() - F.bcols()) {
        const auto Srows = F.rows() * T1bcols;
        t1calc(S, Srows, T1, sel, F.rows(), F.bcols(), abar);
        break; // FIXME
    }
}

} // namespace poly
