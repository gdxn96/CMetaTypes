*Project Overview

-Requirements
--Serialization of Generic C++ objects + members to Json
--De-serialization of Generic C++ objects + members from Json
--Reload object constants from Json

-Future Features?
--Save Entire Game State to Json
--Load Entire Game State from Json
--Link all object variables to lua state automatically
--Override serialization/deserialization methods eg. xml, compressed binary etc.

-Problem Areas
--C++ inherited class members
--std::vector is tricky to create a generic serialization for
--Enums need to be compile time managed
