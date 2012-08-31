# 概要
コンパイルにはGNU MP（Unix Like System : http://gmplib.org, Windows : http://www.mpir.org）が必要となります。  

## 動作確認
* MSVC 10.0  
* Clang 3.2

## cmpxx::integer
GNU MPのmpz_classと同じ働きをします。  
右辺値参照が利用可能になっています。

## cmpxx::rational
GNU MPのmpq_classと同じ働きをします。  
右辺値参照が利用可能になっています。

## cmpxx::fpoint
製作中です。  
cmpxx::integerを利用した、仮数部が多倍精度の事実上どんな巨大な値でも表現できる型になる予定です。