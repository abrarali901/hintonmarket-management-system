#ifndef COMPLIANCEDOCUMENT_H
#define COMPLIANCEDOCUMENT_H

#include <QString>
#include <QDate>

class ComplianceDocument {
public:
    enum class DocumentType {
        BUSINESS_LICENCE,
        LIABILITY_INSURANCE,
        FOOD_HANDLER_CERTIFICATION
    };

    ComplianceDocument();
    ComplianceDocument(DocumentType type, const QString& documentNumber,
                       const QDate& expirationDate);
    ~ComplianceDocument();

    DocumentType getType() const;
    QString getTypeString() const;
    QString getDocumentNumber() const;
    QDate getExpirationDate() const;
    QString getExpirationDateString() const;
    QString getInsuranceProvider() const;

    void setType(DocumentType type);
    void setDocumentNumber(const QString& documentNumber);
    void setExpirationDate(const QDate& expirationDate);
    void setInsuranceProvider(const QString& provider);

    bool isExpired() const;
    bool isValidForSeason() const;

private:
    DocumentType m_type;
    QString m_documentNumber;
    QDate m_expirationDate;
    QString m_insuranceProvider;
};

#endif // COMPLIANCEDOCUMENT_H
