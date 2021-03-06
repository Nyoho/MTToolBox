#ifndef MTTOOLBOX_TEST_LINEARITY_HPP
#define MTTOOLBOX_TEST_LINEARITY_HPP
/**
 * @file TestLinearity.hpp
 *
 *\japanese
 * @brief 疑似乱数生成器がGF(2)線形であるかどうかテストする
 *
 * 疑似乱数生成器がGF(2)線形であるかどうかテストする。テストされる疑似
 * 乱数生成器はEquidistributionCalculatableのサブクラスである必要がある。
 * なお、このテストに落ちればGF(2)線形ではないが、このテストをパスした
 * からといってGF(2)線形であるという保証はない。
 *\endjapanese
 *
 *\english
 * Checks if a pseudo random number generator is GF(2)-linear.
 *
 * This class checks if a pseudo random number generator is
 * GF(2)-linear.  The pseudo random number generator to be tested
 * should be sub class of EquidistributionCalculatable.  Failing the
 * test means the generator is not GF(2)-linear, but passing the test
 * does not assure the generator is GF(2)-linear.
 *\endenglish
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (Hiroshima University)
 *
 * Copyright (C) 2013 Mutsuo Saito, Makoto Matsumoto
 * and Hiroshima University.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */

#include <iostream>
#include <iomanip>
#include <MTToolBox/AbstractGenerator.hpp>
#include <MTToolBox/EquidistributionCalculatable.hpp>

namespace MTToolBox {
    /**
     * @class TestLinearity
     *
     *\japanese
     * @brief 疑似乱数生成器がGF(2)線形であるかどうかテストする
     *
     * テストされる疑似乱数生成器はEquidistributionCalculatableのサブク
     * ラスである必要がある。また、テストされる疑似乱数生成器は状態遷移
     * 関数だけでなく、出力関数もGF(2)線形であることが必要です。
     *
     * なお、このテストに落ちればGF(2)線形ではないが、このテストをパス
     * したからといってGF(2)線形であるという保証はない。
     *
     * @note GF(2)線形に作ったつもりなのに、このテストをパスしなかった場合は、
     * EquidistributionCalculatable#add() の実装に問題があるかもしれな
     * い。
     *
     * @tparam U 疑似乱数生成器の出力の型、符号なし型であること
     *\endjapanese
     *
     *\english
     * Checks if a pseudo random number generator is GF(2)-linear.
     *
     * This class checks if a pseudo random number generator is
     * GF(2)-linear.  The pseudo random number generator to be tested
     * should be sub class of EquidistributionCalculatable.  Failing the
     * test means the generator is not GF(2)-linear, but passing the test
     * does not assure the generator is GF(2)-linear.
     *
     * @note If users are sure that the generator is GF(2)-linear and
     * the generator fails this test, there may be a problem in
     * user's implementation of Equidistribution#add in user's generator.
     * @tparam U type of output of the generator, should be unsigned number.
     *\endenglish
     */
    template<typename U>
    class TestLinearity {
    public:
        /**
         *\japanese
         * generator がGF(2)線形であるかどうかテストする。
         *
         * @param generator テストされる疑似乱数生成器
         * @return true GF(2)線形の場合
         *\endjapanese
         *
         *\english
         * Checks if \b generator is GF(2)-linear.
         *
         * @param generator pseudorandom number generator to be tested
         * @return true \b generator is GF(2)-linear with high probability.
         * @return false \b generator is not GF(2)-linear.
         *\endenglish
         */
        bool operator()(const EquidistributionCalculatable<U>& generator) {
            EquidistributionCalculatable<U> *g1 = generator.clone();
            EquidistributionCalculatable<U> *g2 = generator.clone();
            g1->seed(1234);
            g2->seed(4321);
            bool result = test1(*g1) && test2(*g1, *g2);
            delete g1;
            delete g2;
            return result;
        }

    private:
        bool test1(EquidistributionCalculatable<U>& g1) {
            using namespace std;
            EquidistributionCalculatable<U> *g2 = g1.clone();
            bool result = true;
            g2->add(g1);
            for (int i = 0; i < 100; i++) {
                if (g2->generate() != 0) {
                    result = false;
                    break;
                }
            }
            delete g2;
#if defined(DEBUG)
            if (result) {
                cout << "test1 passed" << endl;
            } else {
                cout << "test1 failed" << endl;
            }
#endif
            return result;
        }

        bool test2(EquidistributionCalculatable<U>& g1,
                   EquidistributionCalculatable<U>& g2) {
            using namespace std;
            EquidistributionCalculatable<U> *g3 = g2.clone();
            g3->add(g1);
            bool result = true;
            for (int i = 0; i < 100; i++) {
                U res1 = g1.generate();
                U res2 = g2.generate();
                U res3 = g3->generate();
                if ((res1 ^ res2) != res3) {
                    result = false;
#if defined(DEBUG)
                    cout << "i,res1,res2,res3 = " << dec << i << ","
                         << hex << res1 << ","
                         << res2 <<"," << res3 << ","
                         << (res1 ^ res2) << endl;
#endif
                    break;
                }
            }
            delete g3;
#if defined(DEBUG)
            if (result) {
                cout << "test2 passed" << endl;
            } else {
                cout << "test2 failed" << endl;
            }
#endif
            return result;
        }

    };
}
#endif // MTTOOLBOX_TEST_LINEARITY_HPP
