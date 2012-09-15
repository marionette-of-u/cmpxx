# 概要
コンパイルにはGNU MP http://gmplib.org が必要となります。  
内部でgmpxx.hを参照している為、GNU MPのビルドの際には  

    ./configure --enable-cxx

を忘れずに！

## 動作確認
* Clang 3.2
* MinGW 32bit
* Ubuntu 32bit

## cmpxx::integer
GNU MPのmpz_classと同じ働きをします。  
右辺値参照が利用可能となっています。  

## cmpxx::rational
GNU MPのmpq_classと同じ働きをします。  
右辺値参照が利用可能となっています。

## cmpxx::floating
GNU MPのmpf_classと同じ働きをします。  
右辺値参照が利用可能となっています。

## 上記3つの共通事項
GNU MPで用いる値型の変数を取得するにはそれぞれ以下の関数を用います。  

    const mpz_class &cmpxx::integer::get_raw_value() const;
    mpz_class &cmpxx::integer::get_raw_value();
    const mpq_class &cmpxx::rational::get_raw_value() const;
    mpq_class &cmpxx::rational::get_raw_value();
    const mpf_class &cmpxx::floating::get_raw_value() const;
    mpf_class &cmpxx::floating::get_raw_value();

また記3つは全て型レベルでのラッパーのため、上述の関数で内部の値に対して如何なる変更をしようとも追加で必要な設定項目が現れたりはしません。

## cmpxx::polynomial
一変数多項式です。  
