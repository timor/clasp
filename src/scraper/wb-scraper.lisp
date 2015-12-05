(in-package :cscrape)

(setf *default-pathname-defaults* (pathname "/home/meister/Dev/clasp/src/scraper/"))
(push :wb-scraper *features*)
(load "scraper.lisp")

(do-scraping (list (sb-posix:getenv "CLASP_CLANG_PATH") "/home/meister/Dev/clasp/src/main/" "/home/meister/Dev/clasp/src/main/include/application.config" "/tmp/all-commands_test.txt" "/tmp/commands_test.txt") :run-preprocessor nil)

(defparameter *all-cc* (read-compile-commands "/tmp/all-commands_test.txt"))
(defparameter *bufs* (read-entire-file (car *all-cc*)))
(length (buffer *bufs*))
(file-position (buffer-stream *bufs*) 12027800)
(search-for-tag *bufs*)
search-for-tag from 12027800
DEBUG found tag at 12028821  new file-position 12028846
DEBUG peek |"BEGIN_TAG_bfc54f90bafadf5 LISP_EXTERNAL_CLASS \"llvmo\" \"LlvmoPkg\" \"llvm::VectorType\" \"VectorType_O\" \"VectorType\" \"SequentialType_O\" END_TAG_bfc54f90bafadf5"|
12028821
12028846
(search-for-tag *bufs*)
search-for-tag from 12028846
DEBUG found tag at 12037643  new file-position 12037668
DEBUG peek |"BEGIN_TAG_bfc54f90bafadf5 NAMESPACE_PACKAGE_ASSOCIATION asttooling AstToolingPkg \"AST-TOOLING\" END_TAG_bfc54f90bafadf5"|
12037643
12037668


(file-position (buffer-stream *bufs*) 12027800)
(next-tag-name *bufs*)
(buffer-peek *bufs*)

(defparameter *nt* nil)
(defparameter *tag-handlers* (tags:make-handler-hash-table))
(file-position (buffer-stream *bufs*) 12027800)
(setq *nt* (next-tag-name *bufs*))
(print "About to parse-tag")
(parse-tag *bufs* *nt* *tag-handlers*)
           (buffer-peek *bufs*)




(length cscrape:*debug-tags*)
(dolist (tag cscrape:*debug-tags*)
  (cond
    ((typep tag 'tags:namespace-package-association-tag)
     (format t "namespace-package-association-tag  namespace: ~a package: ~a~%"
             (tags:namespace tag)
             (tags:package-var tag)))
    ((typep tag 'tags:symbol-tag)
     (format t "symbol-tag  ~a~%"
             (tags:name% tag)))
    ))


(apropos "getenv")
(print "Hello")

(length cscrape::*tags*)
(mapc (lambda (ta)
        (when (typep ta 'tags:symbol-tag)
          (print ta)))
      *tags*)

(mapc (lambda (s)
        (print (tags:name% s)))
      (gethash "KeywordPkg" *symbols-by-package*))

(gethash "ClPkg" *symbols-by-package*)


(maphash (lambda (k v)
           (print k))
         *unique-symbols*)
(defparameter *sbp* (make-hash-table :test #'equal))
(maphash (lambda (key tag)
           (format t "key: ~s   value: ~a~%" key tag)
           (when (typep tag 'tags:symbol-tag)
             (format t "Pushing ~a to (gethash ~a *sbp*)~%" (tags:name% tag) (tags:package% tag))
             (push tag (gethash (tags:package% tag) *sbp*))))
         *unique-symbols*)

(gethash (

(print "Done")



(
(untrace)


(defparameter *ht* (make-hash-table))
(push 'c (gethash 'a *ht*))
*ht*


