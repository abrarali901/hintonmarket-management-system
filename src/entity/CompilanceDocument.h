#ifndef COMPLIANCEDOCUMENT_H
#define COMPLIANCEDOCUMENT_H

#include <QString>
#include <QDate>

class ComplianceDocument {
public:
    // 3 types of Compliance Documents
    enum class DocumentType {
        BUSINESS_LICENCE,
        LIABILITY_INSURANCE,
        FOOD_HANDLER_CERTIFICATION
    };

    ComplianceDocument();
    ComplianceDocument(DocumentType type, const QString& documentNumber,
                       const QDate& expirationDate);
    ~ComplianceDocument();

    // Getters
    DocumentType getType() const;
    QString getTypeString() const;
    QString getDocumentNumber() const;
    QDate getExpirationDate() const;
    QString getExpirationDateString() const;
    QString getInsuranceProvider() const;

    // Setters
    void setType(DocumentType type);
    void setDocumentNumber(const QString& documentNumber);
    void setExpirationDate(const QDate& expirationDate);
    void setInsuranceProvider(const QString& provider);

    // Validation
    bool isExpired() const;
    bool isValidForSeason() const;

private:
    // m_type can only be the one of the allocated enums
    DocumentType m_type;
    QString m_documentNumber;
    QDate m_expirationDate;
    // Only for LIABILITY_INSURANCE - to store provider name
    QString m_insuranceProvider;
};

#endif // COMPLIANCEDOCUMENT_H