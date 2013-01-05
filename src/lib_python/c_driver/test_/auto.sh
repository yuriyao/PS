cd ../
gcc -fPIC -shared -o test_/psimport.so import_module.c ../../util/log.c ../../lib/mm.c ../../lib/web.c -lpython2.6 -lcurl
cd test_
python web_test.py
