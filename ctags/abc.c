// abc.c - Lee Savide - ctags parser for abc music notation
/* 
 * Copyright 2013 Lee Savide
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
const char *name;               /* name of language */
kindOption *kinds;              /* tag kinds handled by parser */
unsigned int kindCount;         /* size of `kinds' list */
const char *const *extensions;  /* list of default extensions */
const char *const *patterns;    /* list of default file name patterns */
parserInitialize initialize;    /* initialization routine, if needed */
simpleParser parser;            /* simple parser (common case) */
rescanParser parser2;           /* rescanning parser (unusual case) */
boolean regex;                  /* is this a regex parser? */

