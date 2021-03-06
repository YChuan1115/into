/* This file is part of Into.
 * Copyright (C) Intopii 2013.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#ifndef _PIICLOCK_H
#define _PIICLOCK_H

#include <PiiDefaultOperation.h>
#include <QDateTime>
#include <QTimer>

/**
 * An operation that emits current time whenever a trigger is
 * received or whenever a timer times out.
 *
 * Inputs
 * ------
 *
 * @in trigger - the trigger. Any object type.
 *
 * Outputs
 * -------
 *
 * @out time - the time as a QString, formatted according to [format].
 *
 * @out timestamp - the unix timestamp.
 *
 */
class PiiClock : public PiiDefaultOperation
{
  Q_OBJECT

  /**
   * The type of the `trigger` input. This property determines how
   * the operation interprets objects received in the `trigger`
   * input. The default value is `TriggerInput`.
   */
  Q_PROPERTY(InputType inputType READ inputType WRITE setInputType);
  Q_ENUMS(InputType);

  /**
   * Set a type of the output time. Supported time types:
   *
   * - `LocalTime` - Locale dependent time (Timezones and Daylight
   * Savings Time). This is the default.
   *
   * - `UTC` - Coordinated Universal Time, replaces Greenwich Mean
   * Time.
   */
  Q_PROPERTY(TimeType timeType READ timeType WRITE setTimeType);
  Q_ENUMS(TimeType);

  /**
   * Set a format of the output.
   *
   * These expressions may be used for the date:
   * - d    - the day as number without a leading zero (1 to 31)
   * - dd   - the day as number with a leading zero (01 to 31)
   * - ddd  - the abbreviated localized day name (e.g. 'Mon' to 'Sun').
   * - dddd - the long localized day name (e.g. 'Qt::Monday' to 'Qt::Sunday').
   * - M    - the month as number without a leading zero (1-12)
   * - MM   - the month as number with a leading zero (01-12)
   * - MMM  - the abbreviated localized month name (e.g. 'Jan' to 'Dec').
   * - MMMM - the long localized month name (e.g. 'January' to 'December').
   * - yy   - the year as two digit number (00-99)
   * - yyyy - the year as four digit number (1752-8000)
   *
   * These expressions may be used for the time:
   * - h    - the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)
   * - hh   - the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)
   * - m    - the minute without a leading zero (0 to 59)
   * - mm   - the minute with a leading zero (00 to 59)
   * - s    - the second whithout a leading zero (0 to 59)
   * - ss   - the second whith a leading zero (00 to 59)
   * - z    - the milliseconds without leading zeroes (0 to 999)
   * - zzz  - the milliseconds with leading zeroes (000 to 999)
   * - AP   - use AM/PM display. AP will be replaced by either "AM" or "PM".
   * - ap   - use am/pm display. ap will be replaced by either "am" or "pm".
   *
   *
   * Example format strings (assuming the date and time is 21 May 2001 14:13:09):
   *
   * <table>
   * <tr><th>Format</th><th>Result</th></tr>
   * <tr><td>dd.MM.yyyy</td><td>21.05.2001</td></tr>
   * <tr><td>ddd MMMM d yy</td><td>Tue May 21 01</td></tr>
   * <tr><td>hh:mm:ss.zzz</td><td>14:13:09.042</td></tr>
   * <tr><td>h:m:s ap</td><td>2:13:9 pm</td></tr>
   * </table>
   *
   * The default format is hh:mm:ss.
   */
  Q_PROPERTY(QString format READ format WRITE setFormat);

  /**
   * An interval to the emit time and timestamp if the trigger input
   * is not connected. The default values is 1000 ms.
   */
  Q_PROPERTY(int interval READ interval WRITE setInterval);

  PII_OPERATION_SERIALIZATION_FUNCTION
public:
  PiiClock();

  /**
   * Supported input types.
   *
   * - `TriggerInput` - Incoming objects trigger an emission of the
   * current time. The type of the incoming object is ignored.
   *
   * - `TimeStampInput` - The input is read as an unsigned int and
   * interpreted as a Unix timestamp. The output will be a formatted
   * version of the time stamp.
   */
  enum InputType { TriggerInput, TimeStampInput };

  /**
   * Supported time types.
   *
   * - `LocalTime` - Locale dependent time (Timezones and Daylight Savings Time).
   *
   * - `UTC` - Coordinated Universal Time, replaces Greenwich Mean
   * Time.
   */
  enum TimeType { LocalTime, UTC };

  void setInputType(const InputType& inputType);
  InputType inputType() const;

  QString format() const;
  void setFormat(QString format);

  void setTimeType(const TimeType& timeType);
  TimeType timeType() const;

  void setInterval(int interval);
  int interval() const;

protected:
  void check(bool reset);
  void process();
  void aboutToChangeState(State state);

private slots:
  void timerTimeout();

private:
  void emitObjects();

  /// @internal
  class Data : public PiiDefaultOperation::Data
  {
  public:
    Data();
    QDateTime dateTime;
    InputType inputType;
    QString strFormat;
    TimeType timeType;
    QTimer timer;
    bool bUseTimer, bTimeOutputConnected, bTimestampOutputConnected;
    PiiOutputSocket* pTimeOutput, *pTimeStampOutput;
  };
  PII_D_FUNC;
};


#endif //_PIICLOCK_H
