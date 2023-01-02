/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2023 Inesonic, LLC.
*
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This file implements configuration helper functions.
***********************************************************************************************************************/

#include <QCoreApplication>

#include <cstdint>
#include <string>

#if (defined(Q_OS_WIN))

    #include <Windows.h>

#endif

#include <ud_usage_data.h>

#include <model_api_types.h>

#include "ld_common.h"
#include "ld_handle.h"
#include "ld_plug_in_manager.h"
#include "ld_plug_in_registrar.h"
#include "ld_variable_name.h"
#include "ld_function_variant.h"
#include "ld_function_data.h"
#include "ld_function_database.h"
#include "ld_data_type.h"
#include "ld_configure.h"

#define TR(_x) QCoreApplication::translate("ConfigureHelpers", (_x))

namespace Ld {
    const DataType::ValueType scalarTypes[5] = {
        DataType::ValueType::BOOLEAN,
        DataType::ValueType::INTEGER,
        DataType::ValueType::REAL,
        DataType::ValueType::COMPLEX,
        DataType::ValueType::NONE
    };


    QList<FunctionVariant> generateVariants(
            DataType::ValueType returnType,
            DataType::ValueType matrixReturnType
        ) {
        QList<FunctionVariant> variants;

        if (returnType != DataType::ValueType::NONE) {
            variants.append(FunctionVariant(returnType, DataType::ValueType::BOOLEAN, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::INTEGER, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::REAL, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::COMPLEX, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::SET, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::TUPLE, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::MATRIX_BOOLEAN, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::MATRIX_INTEGER, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::MATRIX_REAL, TR("any value")));
            variants.append(FunctionVariant(returnType, DataType::ValueType::MATRIX_COMPLEX, TR("any value")));
        }

        if (matrixReturnType != DataType::ValueType::NONE) {
            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    variants.append(
                        FunctionVariant(
                            matrixReturnType,
                            *t1, TR("number rows"),
                            *t2, TR("number columns")
                        )
                    );
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            const QString&      parameterName1,
            bool                parameter1Optional,
            DataType::ValueType returnType,
            DataType::ValueType matrixReturnType
        ) {
        QList<FunctionVariant> variants;

        if (returnType != DataType::ValueType::NONE) {
            if (parameter1Optional) {
                variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                variants.append(FunctionVariant(returnType, *t1, parameterName1));
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (matrixReturnType != DataType::ValueType::NONE) {
            if (parameter1Optional) {
                const DataType::ValueType* t1 = scalarTypes;
                do {
                    const DataType::ValueType* t2 = scalarTypes;
                    do {
                        variants.append(
                             FunctionVariant(
                                matrixReturnType,
                                *t1, TR("number rows"),
                                *t2, TR("number columns")
                            )
                        );
                        ++t2;
                    } while (*t2 != DataType::ValueType::NONE);
                    ++t1;
                } while (*t1 != DataType::ValueType::NONE);
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    const DataType::ValueType* t3 = scalarTypes;
                    do {
                        variants.append(
                            FunctionVariant(
                                matrixReturnType,
                                *t1, TR("number rows"),
                                *t2, TR("number columns"),
                                *t3, parameterName1
                            )
                        );
                        ++t3;
                    } while (*t3 != DataType::ValueType::NONE);
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            const QString&      parameterName1,
            bool                parameter1Optional,
            const QString&      parameterName2,
            bool                parameter2Optional,
            DataType::ValueType returnType,
            DataType::ValueType matrixReturnType
        ) {
        QList<FunctionVariant> variants;

        if (returnType != DataType::ValueType::NONE) {
            if (parameter1Optional && parameter2Optional) {
                variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
            }

            if (parameter2Optional) {
                const DataType::ValueType* t = scalarTypes;
                do {
                    variants.append(FunctionVariant(returnType, *t, parameterName1));
                    ++t;
                } while (*t != DataType::ValueType::NONE);
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    variants.append(FunctionVariant(returnType, *t1, parameterName1, *t2, parameterName2));
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (matrixReturnType != DataType::ValueType::NONE) {
            if (parameter2Optional) {
                const DataType::ValueType* t1 = scalarTypes;
                do {
                    const DataType::ValueType* t2 = scalarTypes;
                    do {
                        const DataType::ValueType* t3 = scalarTypes;
                        do {
                            variants.append(
                                FunctionVariant(
                                    matrixReturnType,
                                    *t1, TR("number rows"),
                                    *t2, TR("number columns"),
                                    *t3, parameterName1
                                )
                            );
                            ++t3;
                        } while (*t3 != DataType::ValueType::NONE);
                        ++t2;
                    } while (*t2 != DataType::ValueType::NONE);
                    ++t1;
                } while (*t1 != DataType::ValueType::NONE);
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    const DataType::ValueType* t3 = scalarTypes;
                    do {
                        const DataType::ValueType* t4 = scalarTypes;
                        do {
                            variants.append(
                                FunctionVariant(
                                    matrixReturnType,
                                    *t1, TR("number rows"),
                                    *t2, TR("number columns"),
                                    *t3, parameterName1,
                                    *t4, parameterName2
                                )
                            );
                            ++t4;
                        } while (*t4 != DataType::ValueType::NONE);
                        ++t3;
                    } while (*t3 != DataType::ValueType::NONE);
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            const QString&      parameterName1,
            bool                parameter1Optional,
            const QString&      parameterName2,
            bool                parameter2Optional,
            const QString&      parameterName3,
            bool                parameter3Optional,
            DataType::ValueType returnType,
            DataType::ValueType matrixReturnType
        ) {
        QList<FunctionVariant> variants;

        if (returnType != DataType::ValueType::NONE) {
            if (parameter1Optional && parameter2Optional) {
                variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
            }

            if (parameter2Optional) {
                const DataType::ValueType* t = scalarTypes;
                do {
                    variants.append(FunctionVariant(returnType, *t, parameterName1));
                    ++t;
                } while (*t != DataType::ValueType::NONE);
            }

            if (parameter3Optional) {
                const DataType::ValueType* t1 = scalarTypes;
                do {
                    const DataType::ValueType* t2 = scalarTypes;
                    do {
                        variants.append(FunctionVariant(returnType, *t1, parameterName1, *t2, parameterName2));
                        ++t2;
                    } while (*t2 != DataType::ValueType::NONE);
                    ++t1;
                } while (*t1 != DataType::ValueType::NONE);
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    const DataType::ValueType* t3 = scalarTypes;
                    do {
                        variants.append(
                            FunctionVariant(
                                returnType,
                                *t1, parameterName1,
                                *t2, parameterName2,
                                *t3, parameterName3
                            )
                        );
                        ++t3;
                    } while (*t3 != DataType::ValueType::NONE);
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (matrixReturnType != DataType::ValueType::NONE) {
            if (parameter2Optional) {
                const DataType::ValueType* t1 = scalarTypes;
                do {
                    const DataType::ValueType* t2 = scalarTypes;
                    do {
                        const DataType::ValueType* t3 = scalarTypes;
                        do {
                            variants.append(
                                FunctionVariant(
                                    matrixReturnType,
                                    *t1, TR("number rows"),
                                    *t2, TR("number columns"),
                                    *t3, parameterName1
                                )
                            );
                            ++t3;
                        } while (*t3 != DataType::ValueType::NONE);
                        ++t2;
                    } while (*t2 != DataType::ValueType::NONE);
                    ++t1;
                } while (*t1 != DataType::ValueType::NONE);
            }

            if (parameter3Optional) {
                const DataType::ValueType* t1 = scalarTypes;
                do {
                    const DataType::ValueType* t2 = scalarTypes;
                    do {
                        const DataType::ValueType* t3 = scalarTypes;
                        do {
                            const DataType::ValueType* t4 = scalarTypes;
                            do {
                                variants.append(
                                    FunctionVariant(
                                        matrixReturnType,
                                        *t1, TR("number rows"),
                                        *t2, TR("number columns"),
                                        *t3, parameterName1,
                                        *t4, parameterName2
                                    )
                                );
                                ++t4;
                            } while (*t4 != DataType::ValueType::NONE);
                            ++t3;
                        } while (*t3 != DataType::ValueType::NONE);
                        ++t2;
                    } while (*t2 != DataType::ValueType::NONE);
                    ++t1;
                } while (*t1 != DataType::ValueType::NONE);
            }

            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    const DataType::ValueType* t3 = scalarTypes;
                    do {
                        const DataType::ValueType* t4 = scalarTypes;
                        do {
                            const DataType::ValueType* t5 = scalarTypes;
                            do {
                                variants.append(
                                    FunctionVariant(
                                        matrixReturnType,
                                        *t1, TR("number rows"),
                                        *t2, TR("number columns"),
                                        *t3, parameterName1,
                                        *t4, parameterName2,
                                        *t5, parameterName3
                                    )
                                );
                                ++t5;
                            } while (*t5 != DataType::ValueType::NONE);
                            ++t4;
                        } while (*t4 != DataType::ValueType::NONE);
                        ++t3;
                    } while (*t3 != DataType::ValueType::NONE);
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            DataType::ValueType returnType,
            const QString&      parameterName1,
            bool                parameter1Optional
        ) {
        QList<FunctionVariant> variants;

        if (parameter1Optional) {
            variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
        }

        const DataType::ValueType* t1 = scalarTypes;
        do {
            variants.append(FunctionVariant(returnType, *t1, parameterName1));
            ++t1;
        } while (*t1 != DataType::ValueType::NONE);

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            DataType::ValueType returnType,
            const QString&      parameterName1,
            bool                parameter1Optional,
            const QString&      parameterName2,
            bool                parameter2Optional
        ) {
        QList<FunctionVariant> variants;

        if (parameter1Optional) {
            assert(parameter2Optional);
            variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
        }

        if (parameter2Optional) {
            const DataType::ValueType* t1 = scalarTypes;
            do {
                variants.append(FunctionVariant(returnType, *t1, parameterName1));
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        const DataType::ValueType* t1 = scalarTypes;
        do {
            const DataType::ValueType* t2 = scalarTypes;
            do {
                variants.append(FunctionVariant(returnType, *t1, parameterName1, *t2, parameterName2));
                ++t2;
            } while (*t2 != DataType::ValueType::NONE);
            ++t1;
        } while (*t1 != DataType::ValueType::NONE);

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            DataType::ValueType returnType,
            const QString&      parameterName1,
            bool                parameter1Optional,
            const QString&      parameterName2,
            bool                parameter2Optional,
            const QString&      parameterName3,
            bool                parameter3Optional
        ) {
        QList<FunctionVariant> variants;

        if (parameter1Optional) {
            assert(parameter2Optional && parameter3Optional);
            variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
        }

        if (parameter2Optional) {
            assert(parameter3Optional);

            const DataType::ValueType* t1 = scalarTypes;
            do {
                variants.append(FunctionVariant(returnType, *t1, parameterName1));
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (parameter3Optional) {
            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    variants.append(FunctionVariant(returnType, *t1, parameterName1, *t2, parameterName2));
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        const DataType::ValueType* t1 = scalarTypes;
        do {
            const DataType::ValueType* t2 = scalarTypes;
            do {
                const DataType::ValueType* t3 = scalarTypes;
                do {
                    variants.append(
                        FunctionVariant(
                            returnType,
                            *t1, parameterName1,
                            *t2, parameterName2,
                            *t3, parameterName3
                        )
                    );
                    ++t3;
                } while (*t3 != DataType::ValueType::NONE);
                ++t2;
            } while (*t2 != DataType::ValueType::NONE);
            ++t1;
        } while (*t1 != DataType::ValueType::NONE);

        return variants;
    }


    QList<FunctionVariant> generateVariants(
            DataType::ValueType returnType,
            const QString&      parameterName1,
            bool                parameter1Optional,
            const QString&      parameterName2,
            bool                parameter2Optional,
            const QString&      parameterName3,
            bool                parameter3Optional,
            const QString&      parameterName4,
            bool                parameter4Optional
        ) {
        QList<FunctionVariant> variants;

        if (parameter1Optional) {
            assert(parameter2Optional && parameter3Optional);
            variants.append(FunctionVariant(returnType, QList<DataType::ValueType>(), QList<QString>()));
        }

        if (parameter2Optional) {
            assert(parameter3Optional);

            const DataType::ValueType* t1 = scalarTypes;
            do {
                variants.append(FunctionVariant(returnType, *t1, parameterName1));
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (parameter3Optional) {
            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    variants.append(FunctionVariant(returnType, *t1, parameterName1, *t2, parameterName2));
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        if (parameter4Optional) {
            const DataType::ValueType* t1 = scalarTypes;
            do {
                const DataType::ValueType* t2 = scalarTypes;
                do {
                    const DataType::ValueType* t3 = scalarTypes;
                    do {
                        variants.append(
                            FunctionVariant(
                                returnType,
                                *t1, parameterName1,
                                *t2, parameterName2,
                                *t3, parameterName3
                            )
                        );
                        ++t3;
                    } while (*t3 != DataType::ValueType::NONE);
                    ++t2;
                } while (*t2 != DataType::ValueType::NONE);
                ++t1;
            } while (*t1 != DataType::ValueType::NONE);
        }

        const DataType::ValueType* t1 = scalarTypes;
        do {
            const DataType::ValueType* t2 = scalarTypes;
            do {
                const DataType::ValueType* t3 = scalarTypes;
                do {
                    const DataType::ValueType* t4 = scalarTypes;
                    do {
                        variants.append(
                            FunctionVariant(
                                returnType,
                                *t1, parameterName1,
                                *t2, parameterName2,
                                *t3, parameterName3,
                                *t4, parameterName4
                            )
                        );
                        ++t4;
                    } while(*t4 != DataType::ValueType::NONE);
                    ++t3;
                } while (*t3 != DataType::ValueType::NONE);
                ++t2;
            } while (*t2 != DataType::ValueType::NONE);
            ++t1;
        } while (*t1 != DataType::ValueType::NONE);

        return variants;
    }
}
